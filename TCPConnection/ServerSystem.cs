using System.Collections;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class ServerSystem : MonoBehaviour
{
    [SerializeField] private int SERVER_PORT = 8888;
    [SerializeField] private string SERVER_IP = "127.0.0.1";

    private TcpClient _tcpClient;
    private NetworkStream _networkStream;
    private Thread _thread;

    private void Awake()
    {
        
    }

    private void Start()
    {
        Connect();
    }

    private void Update()
    {
        
    }

    private void Connect()
    {
        _tcpClient = new TcpClient(SERVER_IP, SERVER_PORT);
        _networkStream = _tcpClient.GetStream();

        _thread = new Thread(ReceiveMessages);
        _thread.Start();
    }

    private void ReceiveMessages()
    {
        byte[] data = new byte[1024];
        while(true)
        {
            int t_bytesRead = _networkStream.Read(data, 0, data.Length);
            string t_message = Encoding.UTF8.GetString(data, 0, t_bytesRead);
            Debug.Log("Received Message from Server : " + t_message);
        }
    }

    public void SendMessageToServer()
    {
        byte[] data = Encoding.UTF8.GetBytes("ButtonPress");
        _networkStream.Write(data, 0, data.Length);
    }

    private void OnApplicationQuit()
    {
        _thread.Abort();
        _networkStream.Close();
        _tcpClient.Close();
    }
}
