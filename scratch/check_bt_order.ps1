$b = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Blueprints/AI/AI_Skeleton/BT_MIR_Skeleton.uasset')
$chars = [System.Text.Encoding]::ASCII.GetString($b)
# Find occurrences of Task / Composite names
$names = @('MIR_Attack_Player', 'MIR_CheckMeleeRange', 'BTTask_MoveTo', 'BTT_MIR_RoamAround', 'BTTask_Wait', 'WithinMeleeRange')
foreach ($n in $names) {
    $idx = 0
    while (($idx = $chars.IndexOf($n, $idx)) -ge 0) {
        Write-Output "$n at offset $idx"
        $idx += $n.Length
    }
}
