using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Business.Implementations.Commands;

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

        /// <summary>
        /// Fox namme
        /// </summary>
        public string FoxName { get; set; }

        /// <summary>
        /// Fox identification data
        /// </summary>
        public FoxIdentificationData IdentificationData { get; set; } = new FoxIdentificationData();

        #region Commands

        public readonly GetIdentificationDataCommand  GetIdentificationDataCommand = new GetIdentificationDataCommand();
        public readonly GetFoxNameCommand GetFoxNameCommand = new GetFoxNameCommand();
        public readonly SetDateAndTimeCommand SetFoxDateAndTimeCommand = new SetDateAndTimeCommand();
        public readonly SetFoxNameCommand SetFoxNameCommand = new SetFoxNameCommand();
        public readonly GetProfilesCountCommand GetFoxProfilesCount = new GetProfilesCountCommand();
        public readonly GetProfileNameCommand GetFoxProfileName = new GetProfileNameCommand();

        #endregion

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
