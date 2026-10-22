$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Models/Characters/Enemies/Skeleton/Animations/ABP_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$idx = 0
while (($idx = $chars.IndexOf("isAttacking", $idx)) -ge 0) {
    $snippet = $chars.Substring([Math]::Max(0, $idx - 100), [Math]::Min(300, $chars.Length - $idx + 100))
    $clean = -join ($snippet.ToCharArray() | ForEach-Object { if ([int]$_ -ge 32 -and [int]$_ -le 126) { $_ } else { ' ' } })
    Write-Output "isAttacking at $idx : $clean"
    $idx += 11
}
