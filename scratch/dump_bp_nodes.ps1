$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/Characters/Enemies/Skeleton/BP_MIR_SkeletonBASE.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)

$regex = [System.Text.RegularExpressions.Regex]::Matches($chars, '([A-Za-z0-9_]+Node[A-Za-z0-9_]*|[A-Za-z0-9_]+Event[A-Za-z0-9_]*)')
$seen = @{}
foreach ($m in $regex) {
    if (-not $seen.ContainsKey($m.Value)) {
        $seen[$m.Value] = $true
        Write-Output $m.Value
    }
}
