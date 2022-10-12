using static httpsock.rawhttp;
using static httpclient1.Program;
using static httpclient2.Program;

namespace dotnet
{
    class Program
    {
        static void Main(string[] args)
        {
            string options;
            string ip = args[0];

            if (args.Length < 0)
            {
                Console.WriteLine("\nNeed attacker IP as my first argument!\n");
            }
            else
            {
                Console.WriteLine("\nSelect any options from below: \n");
                Console.WriteLine("----------------------------------\n");
                Console.WriteLine("1----HTTPCLIENT -  HTTP  -  GET \n");
                Console.WriteLine("2----HTTPCLIENT -  HTTP  -  POST \n");
                Console.WriteLine("3----HTTPCLIENT -  HTTPS -  GET \n");
                Console.WriteLine("4----HTTPCLIENT -  HTTPS -  POST \n");
                Console.WriteLine("5----WEBREQUEST -  HTTP  -  GET \n");
                Console.WriteLine("6----WEBREQUEST -  HTTP  -  POST \n");
                Console.WriteLine("7----WEBREQUEST -  HTTPS -  GET \n");
                Console.WriteLine("8----WEBREQUEST -  HTTPS -  POST \n");
                Console.WriteLine("9----SOCKET     -  HTTP  -  GET \n");
                Console.WriteLine("10---SOCKET     -  HTTP  -  POST \n");
                Console.WriteLine("11---SOCKET     -  HTTPS -  GET \n");
                Console.WriteLine("12---SOCKET     -  HTTPS -  POST \n");
                Console.WriteLine("----------------------------------\n");

                Console.WriteLine("\nSelect:");
                options = Console.ReadLine();

                switch (options)
                {
                    case "1":
                        client(ip, "No", "Get");
                        break;
                    case "2":
                        client(ip, "No", "Post");
                        break;
                    case "3":
                        client(ip, "Yes", "Get");
                        break;
                    case "4":
                        client(ip, "Yes", "Post");
                        break;
                    case "5":
                        webreqget(ip, "No");
                        break;
                    case "6":
                        webreqpost(ip, "No");
                        break;
                    case "7":
                        webreqget(ip, "Yes");
                        break;
                    case "8":
                        webreqpost(ip, "Yes");
                        break;
                    case "9":
                        sockhttp(ip, "Get");
                        break;
                    case "10":
                        sockhttp(ip, "Post");
                        break;
                    case "11":
                        sockhttps(ip, "Get");
                        break;
                    case "12":
                        sockhttps(ip, "Post");
                        break;
                    default:
                        Console.WriteLine("Invalid option!");
                        break;
                }
            }
        }
    }
}