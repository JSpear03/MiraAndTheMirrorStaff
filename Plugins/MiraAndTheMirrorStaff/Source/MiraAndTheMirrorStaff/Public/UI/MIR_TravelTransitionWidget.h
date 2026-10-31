// Copyright Jared Eben Spear, 2026. Licensed for use in "Alumni Arcade."
// All rights reserved.


#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MIR_TravelTransitionWidget.generated.h"

class APlayerController;
class SBorder;
class UTextBlock;

/**
* CURRENTLY UNUSUED.
* Style a child Widget Blueprint; room readiness and fades remain owned by the transition.
*/
UCLASS(Blueprintable, meta = (DisplayName = "MIR Loading Screen"))
class MIRAANDTHEMIRRORSTAFF_API UMIR_TravelTransitionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UMIR_TravelTransitionWidget(const FObjectInitializer& ObjectInitializer);

	/** Shared factory using the current Director's chosen class, with a native fallback. */
	static UMIR_TravelTransitionWidget* CreateLoadingScreen(APlayerController* PlayerController,
		TSubclassOf<UMIR_TravelTransitionWidget> LoadingScreenClass);

	/**
	 * Updates the widget's render opacity, which will fade the entire Designer tree and backing
	 * together--including custom animations.
	 * Additionally, `InTime` is assigned to `AnimationTime` for use in `NativePaint()`.
	 * 
	 * @param InOpacity A value to be assigned to the widget's render opacity during transitions.
	 * Will be clamped between 0.0 and 1.0.
	 * @param InTime A period of time whereby the transition will animate.
	 */
	void SetTransitionVisual(float InOpacity, float InTime);

	/**
	 * Sets `InStatus` to `LoadingStatus`, updating `LoadingStatusText` accordingly.
	 * 
	 * @param InStatus The text to assign to `LoadingStatus` and thus display in `LoadingStatusText`.
	 * If its value is equal to `LoadingStatus` (e.g., both have the same text), the function
	 * exits immediately.
	 */
	void SetLoadingStatus(const FText& InStatus);

	/**
	 * Starts (or restarts) the UWidget interface.
	 * Afterward, the loading style is applied.
	 */
	virtual void SynchronizeProperties() override;

	/**
	 * Determines whether to release the Slate resources. If so, it thereby starts (or restarts)
	 * the UVisual interface.
	 * Afterward, the widget's background is reset so that it can be drawn to anew.
	 * 
	 * @param bReleaseChildren Whether to release the Slate resources.
	 */
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	/** Turn off when supplying your own images, text and animation in the Designer. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIR|Loading Screen")
	bool bShowDefaultArtwork = true;

	/** Solid backing behind your design. Alpha is always opaque while loading. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MIR|Loading Screen", meta = (HideAlphaChannel))
	FLinearColor BackgroundColor = FLinearColor(0.007f, 0.008f, 0.019f, 1.f);

	/** Bind a Text Block to this, or name it LoadingStatusText for automatic updates. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "MIR|Loading Screen")
	FText LoadingStatus;

	/** Overall screen opacity, controlled by the travel fade (not loading progress). */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "MIR|Loading Screen")
	float TransitionOpacity = 0.f;

	/** Elapsed transition seconds, available for custom visual effects. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "DUA|Loading Screen")
	float AnimationTime = 0.f;

protected:

	/** Handles the widget preconstruct, updating the loading style when appropriate. */
	virtual void NativePreConstruct() override;

	/** Handles the appearance of the widget's background (padding, alignment, etc.). */
	virtual TSharedRef<SWidget> RebuildWidget() override;

	/** Handles the drawing of custom graphics onto the widget, such as arcs and centered text.
	* 
	* @param Args The widget's owning player, world, etc.
	* @param Geometry The full geometry of the widget in world/screen space.
	* @param CullingRect The culling rectangle for this widget's draw call.
	* @param Elements Output list of `FSlateDrawElement` objects to render.
	* @param Layer The LayerID of the render.
	* @param Style Styling resources like font, colors, etc.
	* @param bParentEnabled Whether the parent widget is enabled.
	* 
	* @returns The maximum LayerID painted on.
	*/
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& Geometry,
		const FSlateRect& CullingRect, FSlateWindowElementList& Elements, int32 Layer,
		const FWidgetStyle& Style, bool bParentEnabled) const override;

	/** A text block containing the status of the load. */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> LoadingStatusText;

private:
	/**
	 * First, handles the setting of the background color and displayed loading-status text.
	 * Then, it sets the render opacity according to whether the widget is being displayed
	 * in the designer.
	 */
	void ApplyLoadingStyle();

	/** A border that serves as the widget's background. */
	TSharedPtr<SBorder> Background;
};
