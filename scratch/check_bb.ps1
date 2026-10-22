$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/AI/AI_Skeleton/BB_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$matches = [System.Text.RegularExpressions.Regex]::Matches($chars, '[\x20-\x7E]{4,}')
foreach ($m in $matches) {
    if ($m.Value -match '(?i)(CanAttack|Attack|Melee|Range|Player|Target|Seen)') {
        Write-Output $m.Value
    }
}
