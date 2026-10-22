$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/AI/AI_Skeleton/BT_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
# Match all NodeName or node titles
$regex = [System.Text.RegularExpressions.Regex]::Matches($chars, 'NodeName\x00[^\x00]+\x00([^\x00]+)')
foreach ($m in $regex) {
    Write-Output $m.Value
}
