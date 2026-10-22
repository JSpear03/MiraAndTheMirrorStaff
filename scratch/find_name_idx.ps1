$bytes = [System.IO.File]::ReadAllBytes('Plugins/MiraAndTheMirrorStaff/Content/Models/Characters/Enemies/Skeleton/Animations/ABP_MIR_Skeleton.uasset')

# Find string in the name table portion:
# In the name table, strings are stored with length prefix.
# Let's find "isAttacking" in the names:
$chars = [System.Text.Encoding]::ASCII.GetString($bytes)
$nameTableNames = @()
# Let's find where the names start:
# In dump_abp earlier, names started at a certain offset.
# Let's search for "None" which is always name 0.
$noneIdx = 0
$idx = $chars.IndexOf("None")
Write-Output "None at $idx"

# Let's locate the name table by reading sequentially from the header
# In UE5 uassets, let's look at the summary at offset 0
$reader = New-Object System.IO.BinaryReader(New-Object System.IO.MemoryStream(,$bytes))
$tag = $reader.ReadUInt32() # 0x9E2A83C1
$reader.BaseStream.Position = 0
$buf = $reader.ReadBytes(1024)
# Let's find name "isAttacking" offset
$isAttackStrIdx = $chars.IndexOf("isAttacking")
Write-Output "isAttacking in file at $isAttackStrIdx"

# Let's count how many length-prefixed strings are between None and isAttacking
