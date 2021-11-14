using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Models;
using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    /// <summary>
    /// Able to connect us to a fox
    /// </summary>
    public class FoxConnector : IFoxConnector
    {
        public async Task ConnectAsync(MainModel model, PairedFoxDTO foxToConnect)
        {
            _ = model ?? throw new ArgumentNullException(nameof(model));
            _ = foxToConnect ?? throw new ArgumentNullException(nameof(foxToConnect));

            // TODO: Put actual connecting code here
            Debug.WriteLine($"Connecting to fox: {foxToConnect.DisplayName}");

            model.ConnectedFox = foxToConnect;
            model.IsConnected = true;
        }

        public async Task DisconnectAsync(MainModel model)
        {
            _ = model ?? throw new ArgumentException(nameof(model));

            // Put actual disconnection code here

            model.IsConnected = false;
            model.ConnectedFox = null;
        }
    }
}
