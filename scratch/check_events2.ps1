$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/Characters/Enemies/Skeleton/BP_MIR_SkeletonBASE.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
for ($i = 0; $i -le 5; $i++) {
    $target = "K2Node_Event_$i"
    $idx = $chars.IndexOf($target)
    if ($idx -ge 0) {
        $snippet = $chars.Substring($idx, [Math]::Min(300, $chars.Length - $idx))
        $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
        Write-Output "$target : $clean"
    }
}
