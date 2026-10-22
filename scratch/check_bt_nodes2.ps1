$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/AI/AI_Skeleton/BT_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
$matches = [System.Text.RegularExpressions.Regex]::Matches($chars, '[\x20-\x7E]{4,}')
for ($i = 0; $i -lt $matches.Count; $i++) {
    $val = $matches[$i].Value
    if ($val -match '(?i)(Selector|Sequence|Task|Decorator|MoveTo|Wait|Roam|Check|Attack|Range|Chase|Look|Player)') {
        Write-Output $val
    }
}
