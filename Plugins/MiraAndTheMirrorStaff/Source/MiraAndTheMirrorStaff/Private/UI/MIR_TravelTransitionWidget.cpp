// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#include "UI/MIR_TravelTransitionWidget.h"
#include "Components/TextBlock.h"
#include "Fonts/FontMeasure.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/PlayerController.h"
#include "Rendering/DrawElements.h"
#include "Rendering/SlateRenderer.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"

#define LOCTEXT_NAMESPACE "MIR_TravelLoading"

UMIR_TravelTransitionWidget::UMIR_TravelTransitionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LoadingStatus = LOCTEXT("LoadingDefault", "Preparing the world...");
}

UMIR_TravelTransitionWidget* UMIR_TravelTransitionWidget::CreateLoadingScreen(APlayerController* PlayerController,
	TSubclassOf<UMIR_TravelTransitionWidget> LoadingScreenClass)
{
	// Validate `PlayerController`.
	if (!IsValid(PlayerController)) return nullptr;

	// Dereference `LoadingScreenClass` to a UClass and validate it before widget creation.
	/* A class made by dereferencing `LoadingScreenClass` to a UClass. */
	UClass* WidgetClass = LoadingScreenClass.Get();
	if (!WidgetClass || !WidgetClass->IsChildOf(StaticClass())
		|| WidgetClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists))
	{
		WidgetClass = StaticClass();
	}

	// Create the widget from the class defined above.
	/* A widget created from `WidgetClass`, defined above. */
	UMIR_TravelTransitionWidget* Widget = CreateWidget<UMIR_TravelTransitionWidget>(PlayerController, WidgetClass);
	if (!Widget && WidgetClass != StaticClass())
	{
		Widget = CreateWidget<UMIR_TravelTransitionWidget>(PlayerController, StaticClass());
	}
	if (Widget) Widget->SetTransitionVisual(0.f, 0.f);

	return Widget;
}

TSharedRef<SWidget> UMIR_TravelTransitionWidget::RebuildWidget()
{
	// NativePaint runs after the Designer tree. Put the opaque cover behind that tree
	// so a custom layout can draw freely without exposing the unfinished room.
	Background = SNew(SBorder)
		.Padding(0.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderImage(FCoreStyle::Get().GetBrush(TEXT("WhiteBrush")))
		.BorderBackgroundColor(FLinearColor(BackgroundColor.R, BackgroundColor.G, BackgroundColor.B, 1.f))
		[Super::RebuildWidget()];
	return Background.ToSharedRef();
}

void UMIR_TravelTransitionWidget::ApplyLoadingStyle()
{
	// Determine background color and loading text.
	if (Background)
	{
		Background->SetBorderBackgroundColor(FLinearColor(BackgroundColor.R, BackgroundColor.G, BackgroundColor.B, 1.f));
	}
	if (LoadingStatusText) LoadingStatusText->SetText(LoadingStatus);

	// Set render opacity based on whether the widget is in design time.
	SetRenderOpacity(IsDesignTime() ? 1.f : TransitionOpacity);
}

void UMIR_TravelTransitionWidget::NativePreConstruct()
{
	ApplyLoadingStyle();
	Super::NativePreConstruct();
	ApplyLoadingStyle();
}

void UMIR_TravelTransitionWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	ApplyLoadingStyle();
}

void UMIR_TravelTransitionWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	Background.Reset();
}

void UMIR_TravelTransitionWidget::SetTransitionVisual(float InOpacity, float InTime)
{
	// Update `AnimationTime` for use in `NativePaint()`.
	AnimationTime = InTime;

	// Fade the entire Designer tree and backing together, including custom animations.
	TransitionOpacity = FMath::Clamp(InOpacity, 0.f, 1.f);
	SetRenderOpacity(TransitionOpacity);

	// Forget current; redraw and recache on next paint pass.
	InvalidateLayoutAndVolatility();
}

void UMIR_TravelTransitionWidget::SetLoadingStatus(const FText& InStatus)
{
	// Prevent assigning an identical value.
	if (LoadingStatus.EqualTo(InStatus)) return;

	// Assign loading-status text where appropriate.
	LoadingStatus = InStatus;
	if (LoadingStatusText) LoadingStatusText->SetText(LoadingStatus);

	// Forget current; redraw and recache on next paint pass.
	InvalidateLayoutAndVolatility();
}

int32 UMIR_TravelTransitionWidget::NativePaint(const FPaintArgs& Args, const FGeometry& Geometry,
	const FSlateRect& CullingRect, FSlateWindowElementList& Elements, int32 Layer,
	const FWidgetStyle& Style, bool bParentEnabled) const
{
	// Slate has already applied the widget's render opacity to this style.
	const float Alpha = Style.GetColorAndOpacityTint().A;
	if (bShowDefaultArtwork && Alpha > KINDA_SMALL_NUMBER && FSlateApplication::IsInitialized())
	{
		// Draw arcs.
		const FVector2D Size(Geometry.GetLocalSize());
		const float Scale = FMath::Clamp(static_cast<float>(FMath::Min(Size.X / 960.0, Size.Y / 540.0)), 0.5f, 1.5f);
		const FVector2D Center(Size.X * 0.5, Size.Y * 0.5 - 48.f * Scale);
		const auto DrawArc = [&](float Start, float Sweep, float Radius, float Width, const FLinearColor& Color)
			{
				TArray<FVector2D> Points;
				constexpr int32 Segments = 48;
				Points.Reserve(Segments + 1);
				for (int32 Index = 0; Index <= Segments; ++Index)
				{
					const float Angle = Start + Sweep * static_cast<float>(Index) / Segments;
					Points.Add(Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Radius * Scale);
				}
				FSlateDrawElement::MakeLines(Elements, Layer + 1, Geometry.ToPaintGeometry(), Points,
					ESlateDrawEffect::None, Color, true, Width * Scale);
			};
		DrawArc(0.f, 2.f * PI, 37.f, 2.f, FLinearColor(0.1f, 0.1f, 0.16f, Alpha));
		const float Rotation = FMath::Fmod(AnimationTime * 1.6f, 2.f * PI);
		DrawArc(Rotation, 2.3f, 37.f, 4.f, FLinearColor(0.87f, 0.93f, 1.f, Alpha));
		DrawArc(Rotation + PI, 2.3f, 37.f, 4.f, FLinearColor(0.42f, 0.26f, 0.75f, Alpha));

		// Draw centered text.
		const auto FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		const auto DrawCenteredText = [&](const FText& Text, float Y, int32 FontSize, const TCHAR* Weight, const FLinearColor& Color)
			{
				const FSlateFontInfo Font = FCoreStyle::GetDefaultFontStyle(Weight, FMath::RoundToInt(FontSize * Scale));
				const FVector2D TextSize(FontMeasure->Measure(Text, Font));
				const FVector2D Position((Size.X - TextSize.X) * 0.5, Y);
				FSlateDrawElement::MakeText(Elements, Layer + 2,
					Geometry.ToPaintGeometry(TextSize, FSlateLayoutTransform(Position)), Text, Font,
					ESlateDrawEffect::None, Color);
			};
		DrawCenteredText(LOCTEXT("LoadingTitle", "Loading next area"), Center.Y + 66.f * Scale,
			24, TEXT("Bold"), FLinearColor(0.93f, 0.95f, 1.f, Alpha));
		DrawCenteredText(LoadingStatus.IsEmpty() ? LOCTEXT("LoadingDefault", "Preparing the world...") : LoadingStatus,
			Center.Y + 105.f * Scale, 14, TEXT("Regular"), FLinearColor(0.6f, 0.64f, 0.76f, Alpha));
		DrawCenteredText(LOCTEXT("GameTitle", "Mira and the Mirror Staff"), Size.Y - 48.f * Scale,
			12, TEXT("Regular"), FLinearColor(0.44f, 0.46f, 0.57f, Alpha));
	}
	return Super::NativePaint(Args, Geometry, CullingRect, Elements, Layer + 3, Style, bParentEnabled);
}

#undef LOCTEXT_NAMESPACE
