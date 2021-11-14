using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    /// <summary>
    /// Able to connect us to a fox
    /// </summary>
    public class FoxConnector : IFoxConnector
    {
        public Task ConnectAsync(MainModel model, PairedFoxDTO foxToConnect)
        {
            // TODO: Put actual connecting code here
            Debug.WriteLine($"Connecting to fox: {foxToConnect.DisplayName}");

            model.ConnectedFox = foxToConnect;
            model.IsConnected = true;

            return Task.CompletedTask;
        }
    }
}
