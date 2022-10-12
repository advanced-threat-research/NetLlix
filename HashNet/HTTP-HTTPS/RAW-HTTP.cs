using System.Net.Security;
using System.Net.Sockets;
using System.Text;

namespace httpsock
{
    public class rawhttp
    {
        public static async void sockhttps(string ip, string method)
        {
            int port = 443;
            string request;

            Console.WriteLine("Using raw sockets in CHash");

            if (method == "Get")
                request = "GET /exfil.txt HTTP/1.1\r\nHost: " + ip + "\r\nConnection: close\r\n\r\n";
            else
                request = "POST / HTTP/1.1\r\nHost: " + ip + "\r\nConnection:close\r\nContent-Length:16\r\nContent-Type:application/x-www-form-urlencoded\r\n\r\nusername=iamroot";

            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            socket.ReceiveTimeout = 10000;
            socket.NoDelay = true;
            socket.SendBufferSize = 8000;
            socket.ReceiveBufferSize = 8000;
            socket.Connect(ip, port);

            var tcpStream = new NetworkStream(socket, true);
            var sslStream = new SslStream(tcpStream, true, (s, cert, chain, policy) => true, null);
            sslStream.AuthenticateAsClientAsync(ip).GetAwaiter().GetResult();
            SendMessage(request, sslStream);
            var msg1 = ReadMessage(sslStream);
            Console.WriteLine($"Received SSL message: {msg1.Substring(0, 10)}...");

            sslStream.Dispose();
            sslStream = null;

            socket.Poll(0, SelectMode.SelectError);

            Console.WriteLine("Done.");
            socket.Close();
            Environment.Exit(0);
        }

        public static async void sockhttp(string ip, string method)
        {
            int port = 80;
            string request;

            Console.WriteLine("Using raw sockets in CHash");

            if (method == "Get")
                request = "GET /exfil.txt HTTP/1.1\r\nHost: " + ip +"\r\nConnection: close\r\n\r\n";
            else
                request = "POST / HTTP/1.1\r\nHost: " + ip +"\r\nConnection:close\r\nContent-Length:16\r\nContent-Type:application/x-www-form-urlencoded\r\n\r\nusername=iamroot";

            Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            socket.ReceiveTimeout = 10000;
            socket.NoDelay = true;
            socket.SendBufferSize = 8000;
            socket.ReceiveBufferSize = 8000;
            socket.Connect(ip, port);

            var tcpStream = new NetworkStream(socket, true);
            
            SendMessage(request, tcpStream);
            var msg1 = ReadMessage(tcpStream);
            Console.WriteLine($"Received PLAIN message: {msg1.Substring(0, 10)}...");
            Console.WriteLine("Done.");
            socket.Close();
            Environment.Exit(0);
        }

        static void SendMessage(string c, Stream stream)
        {
            var bytes = Encoding.UTF8.GetBytes(c);
            stream.Write(bytes, 0, bytes.Length);
        }

        static string ReadMessage(Stream stream)
        {
            // Read the  message sent by the client.
            // The client signals the end of the message using the
            // "<EOF>" marker.
            byte[] buffer = new byte[2048];
            StringBuilder messageData = new StringBuilder();
            int bytes = -1;
            do
            {
                // Read the client's test message.
                bytes = stream.Read(buffer, 0, buffer.Length);

                // Use Decoder class to convert from bytes to UTF8
                // in case a character spans two buffers.
                Decoder decoder = Encoding.UTF8.GetDecoder();
                char[] chars = new char[decoder.GetCharCount(buffer, 0, bytes)];
                decoder.GetChars(buffer, 0, bytes, chars, 0);
                messageData.Append(chars);
                // Check for EOF or an empty message.
                if (messageData.ToString().IndexOf("<EOF>") != -1)
                {
                    break;
                }
            } while (bytes != 0);

            return messageData.ToString();
        }
    }
}