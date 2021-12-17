using org.whitefossa.yiffhl.Abstractions.DTOs;
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

        /// <summary>
        /// Fox namme
        /// </summary>
        public string FoxName { get; set; }

        /// <summary>
        /// Fox identification data
        /// </summary>
        public FoxIdentificationData IdentificationData { get; set; } = new FoxIdentificationData();

        /// <summary>
        /// Current fox profile
        /// </summary>
        public Profile CurrentProfile { get; set; }

        /// <summary>
        /// Current profile settings
        /// </summary>
        public ProfileSettings CurrentProfileSettings { get; set; } = new ProfileSettings();

        /// <summary>
        /// Dynamic fox status
        /// </summary>
        public DynamicFoxStatus DynamicFoxStatus { get; set; } = new DynamicFoxStatus();

        /// <summary>
        /// Static fox status
        /// </summary>
        public StaticFoxStatus StaticFoxStatus { get; set; } = new StaticFoxStatus();

        #region Fox events

        /// <summary>
        /// Delegate, called when fox is armed
        /// </summary>
        public OnFoxArmedEventDelegate OnFoxArmed { get; set; }

        /// <summary>
        /// Delegate, called when new antenna matching measurement event arrives
        /// </summary>
        public OnAntennaMatchingMeasurementEventDelegate OnAntennaMatchingMeasurement { get; set; }

        /// <summary>
        /// Delegate, called when fox going to sleepmode
        /// </summary>
        public OnEnteringSleepmodeEventDelegate OnEnteringSleepmode { get; set; }

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

            OnFoxArmed = null;
            OnAntennaMatchingMeasurement = null;
            OnEnteringSleepmode = null;
        }
    }
}
