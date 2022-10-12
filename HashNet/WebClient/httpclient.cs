using System.Net;
using System.Net.Security;
using System.Text;
using System.Text.Json;

namespace httpclient1
{
    public class Program
    {
        public static void client(string ip, string isSSL, string method)
        {
            var values = new Dictionary<string, string>
            {
                { "username", "alice" },
                { "password", "password" }
            };

            var data = new FormUrlEncodedContent(values);

            Console.WriteLine("Using HTTPCLIENT in CHash");

            HttpClientHandler clientHandler = new HttpClientHandler();
            //ignore the invalid SSl cert error
            clientHandler.ServerCertificateCustomValidationCallback = (sender, cert, chain, sslPolicyErrors) => { return true; };

            using (var client = new HttpClient(clientHandler))
                {
                    if(isSSL == "No")
                        client.BaseAddress = new Uri("http://" + ip);
                    else
                        client.BaseAddress = new Uri("https://" + ip);

                    if (method == "Get")
                    {
                        HttpResponseMessage response = client.GetAsync("/exfil.txt").Result;
                        response.EnsureSuccessStatusCode();
                        string result = response.Content.ReadAsStringAsync().Result;
                        Console.WriteLine("Result: " + result);
                    }
                    else
                    {
                        HttpResponseMessage response = client.PostAsync("/", data).Result;
                        response.EnsureSuccessStatusCode();
                        string result = response.Content.ReadAsStringAsync().Result;
                    }
                }
        }

    }
}
