param( [string] $ip, [int] $port,[string] $method )   

try
{
	Write-Host "Connecting to $ip on port $port ... " -NoNewLine
	try
	{
		$socket = New-Object System.Net.Sockets.TcpClient( $ip, $port )
		Write-Host -ForegroundColor Green "OK"
	}
	
	catch
	{
		Write-Host -ForegroundColor Red "failed"
		exit -1
	}

	$stream = $socket.GetStream()
	$Callback = { param($sender, $cert, $chain, $errors) return $true }
    $SslStream = New-Object System.Net.Security.SslStream($stream, $false, $Callback)
	$sslStream.AuthenticateAsClient('Powershell')
	$writer = New-Object System.IO.StreamWriter($sslStream)
	
	$buffer = New-Object System.Byte[] 1024
	$encoding = New-Object System.Text.AsciiEncoding
	$writer.AutoFlush = $true
	
	if ( $method -eq "GET" )
	{
		$command = "GET / HTTP/1.1`r`nHost: " + $ip +"`r`nConnection: close`r`n`r`n"
	}
	else
	{
		$command = "POST / HTTP/1.1`r`nHost: " + $ip +"`r`nConnection:close`r`nContent-Length:14`r`nContent-Type:application/x-www-form-urlencoded`r`n`r`nusername=nina"
	}
	
	$writer.WriteLine( $command )
	
	while( $socket )
	{
		start-sleep -m 500
		while( $SslStream.DataAvailable )
		{
			$read = $SslStream.Read( $buffer, 0, 1024 )
			Write-Host -n ($encoding.GetString( $buffer, 0, $read ))
		}
		break
	}
	
}

finally
{
	$SslStream.Dispose()
	if( $writer ) {	$writer.Close( )	}
	if( $stream ) {	$stream.Close( )	}
}
