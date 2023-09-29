# This sets the output high.
Set-NetAdapterAdvancedProperty -name "Ethernet" -DisplayName "Speed & Duplex" -DisplayValue "10 Mbps Full Duplex"

sleep 12

# This sets the output low. There's around 4 second delay before things happen. 
Set-NetAdapterAdvancedProperty -name "Ethernet" -DisplayName "Speed & Duplex" -DisplayValue "1.0 Gbps Full Duplex"

sleep 4
