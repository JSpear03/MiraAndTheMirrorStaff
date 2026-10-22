$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/Characters/Enemies/Skeleton/BP_MIR_SkeletonBASE.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$idx = $chars.IndexOf("CallParentFunction_2")
if ($idx -ge 0) {
    $snippet = $chars.Substring($idx, [Math]::Min(1000, $chars.Length - $idx))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "CallParentFunction_2: $clean"
}
