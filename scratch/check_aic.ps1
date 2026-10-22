$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/AI/AI_Skeleton/AIC_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$matches = [System.Text.RegularExpressions.Regex]::Matches($chars, '[\x20-\x7E]{4,}')
$seen = @{}
foreach ($m in $matches) {
    if ($m.Value -match '(?i)(Tree|Blackboard|Run|Behavior|Pawn|Target|Attack|Melee|Range)') {
        if (-not $seen.ContainsKey($m.Value)) {
            $seen[$m.Value] = $true
            Write-Output $m.Value
        }
    }
}
