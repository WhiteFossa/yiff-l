﻿using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;

namespace org.whitefossa.yiffhl.Models
{
    /// <summary>
    /// Main model
    /// </summary>
    public class MainModel
    {
        /// <summary>
        /// Are we connected to a fox?
        /// </summary>
        public bool IsConnected { get; set; }

        /// <summary>
        /// We are connected to this fox (if connected at all)
        /// </summary>
        public PairedFoxDTO ConnectedFox { get; set; }

        /// <summary>
        /// Delegate, called on each new byte from a fox
        /// </summary>
        public OnFoxConnectorNewByteReadDelegate OnFoxConnectorNewByteRead { get; set; }

        /// <summary>
        /// Delegate, called when connection to a fox is estabilished
        /// </summary>
        public OnFoxConnectorConnectedDelegate OnFoxConnectorConnected { get; set; }

        /// <summary>
        /// Delegate, called when connection to a fox is dropped
        /// </summary>
        public OnFoxConnectorDisconnectedDelegate OnFoxConnectorDisconnected { get; set; }

        /// <summary>
        /// Delegate, called when connection to a fox is failed
        /// </summary>
        public OnFoxConnectorFailedToConnectDelegate OnFoxConnectorFailedToConnect { get; set; }

        public MainModel()
        {
            // Initial state
            IsConnected = false;
            ConnectedFox = null;

            OnFoxConnectorNewByteRead = null;
            OnFoxConnectorConnected = null;
            OnFoxConnectorDisconnected = null;
            OnFoxConnectorFailedToConnect = null;
        }
    }
}
