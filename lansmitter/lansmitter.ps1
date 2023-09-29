# Lansmitter
# Transmits data using morse code by encoding it into the Ethernet interface speed :)
# 
# Tune to 999.9725 MHz





function dit {
    Write-Host "." -NoNewline

    # This sets the output high.
    Set-NetAdapterAdvancedProperty -name "Ethernet" -DisplayName "Speed & Duplex" -DisplayValue "10 Mbps Full Duplex"
    
    # This sets the output low. There's around 4 second delay before things happen. 
    Set-NetAdapterAdvancedProperty -name "Ethernet" -DisplayName "Speed & Duplex" -DisplayValue "1.0 Gbps Full Duplex"
    
    sleep 4

}


function dash {
    Write-Host "-" -NoNewline
	
    # This sets the output high.	
    Set-NetAdapterAdvancedProperty -name "Ethernet" -DisplayName "Speed & Duplex" -DisplayValue "10 Mbps Full Duplex"
    sleep 12
    
    # This sets the output low. There's around 4 second delay before things happen. 
    Set-NetAdapterAdvancedProperty -name "Ethernet" -DisplayName "Speed & Duplex" -DisplayValue "1.0 Gbps Full Duplex"
    sleep 4
}


if ($args.Count -eq 0) {
    Write-Host "usage: .\lansmitter <input text>"
} else {
    Write-Host "Lansmitting.. Please stand by.. "

    $charArray = @()  # Initialize an empty array to store characters
    foreach ($arg in $args) {
        $arg = $arg.Trim()  # Remove leading and trailing spaces
        $charArray += $arg.ToCharArray()  # Split the argument into characters
        $charArray += " "  # Add a space between words
    }	
    $charArray = $charArray[0..($charArray.Count - 2)]

    foreach ($char in $charArray) {
        $char = [char]::ToUpper($char)
		Write-Host $char -NoNewLine
		Write-Host ": " -NoNewLine
		
        switch ($char) {
            # Morse code mappings for letters
            "A" {dit; dash}
            "B" {dash; dit; dit; dit}
            "C" {dash; dit; dash; dit}
            "D" {dash; dit; dit}
            "E" {dit}
            "F" {dit; dit; dash; dit}
            "G" {dash; dash; dit}
            "H" {dit; dit; dit; dit}
            "I" {dit; dit}
            "J" {dit; dash; dash; dash}
            "K" {dash; dit; dash}
            "L" {dit; dash; dit; dit}
            "M" {dash; dash}
            "N" {dash; dit}
            "O" {dash; dash; dash}
            "P" {dit; dash; dash; dit}
            "Q" {dash; dash; dit; dash}
            "R" {dit; dash; dit}
            "S" {dit; dit; dit}
            "T" {dash}
            "U" {dit; dit; dash}
            "V" {dit; dit; dit; dash}
            "W" {dit; dash; dash}
            "X" {dash; dit; dit; dash}
            "Y" {dash; dit; dash; dash}
            "Z" {dash; dash; dit; dit}
            " " {Start-Sleep -Milliseconds 1000}  # Space character
            default {Write-Host " " -NoNewline}  # Handle unrecognized characters as space
        }
#        Write-Host "   " -NoNewline
        Write-Host ""
		
        Start-Sleep -Milliseconds 3000  # Pause between characters
    }
    Write-Host ""  # Add a newline after processing all characters
}
