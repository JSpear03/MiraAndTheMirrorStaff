$bytes = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Models/Characters/Enemies/Skeleton/Animations/ABP_MIR_Skeleton.uasset')
$reader = New-Object System.IO.BinaryReader(New-Object System.IO.MemoryStream(,$bytes))
$reader.BaseStream.Position = 4843 - 4 # length prefix of None is 4 bytes before None

$names = @()
while ($reader.BaseStream.Position -lt 12000) {
    $pos = $reader.BaseStream.Position
    $len = $reader.ReadInt32()
    if ($len -gt 0 -and $len -lt 256) {
        $strBytes = $reader.ReadBytes($len - 1)
        $term = $reader.ReadByte()
        $hash = $reader.ReadUInt32() # hash
        $name = [System.Text.Encoding]::ASCII.GetString($strBytes)
        $names += [PSCustomObject]@{ Index = $names.Count; Name = $name; Pos = $pos }
    } else {
        break
    }
}

Write-Output "Parsed $($names.Count) names"
foreach ($n in $names) {
    if ($n.Name -match '(?i)(attack|melee|state|can|isattack|speed|move)') {
        Write-Output "[$($n.Index)] $($n.Name)"
    }
}
