# A super-basic example PowerShell script

Write-Host 'Hello, world!'

Get-Date -Format "dd-MM-yyyy HH:mm"

# List CPU cores on this box
WmiObject -class win32_processor -Property Name, NumberOfCores, NumberOfLogicalProcessors | Format-List -Property Name, NumberOfCores, NumberOfLogicalProcessors 
