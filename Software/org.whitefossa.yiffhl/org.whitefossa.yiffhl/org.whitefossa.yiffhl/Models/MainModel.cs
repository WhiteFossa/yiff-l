﻿using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Models;
using org.whitefossa.yiffhl.ViewModels;
using System.Collections.Generic;

namespace org.whitefossa.yiffhl.Models
{
    /// <summary>
    /// Main model
    /// </summary>
    public class MainModel : IMainModel
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

        /// <summary>
        /// Antenna matching model
        /// </summary>
        public MatchingModel MatchingModel { get; set; } = new MatchingModel();

        /// <summary>
        /// Service settings model
        /// </summary>
        public ServiceSettingsModel ServiceSettingsModel { get; set; } = new ServiceSettingsModel();

        /// <summary>
        /// Stacked app displays
        /// </summary>
        public Stack<ActiveDisplay> AppDisplays { get; set; } = new Stack<ActiveDisplay>();

        #region Fox events

        /// <summary>
        /// Delegate, called when fox is armed
        /// </summary>
        public OnFoxArmedDelegate OnFoxArmed { get; set; }

        /// <summary>
        /// Delegate, called when fox going to sleepmode
        /// </summary>
        public OnEnteringSleepmodeEventDelegate OnEnteringSleepmode { get; set; }

        /// <summary>
        /// Delegate, called when fox is disarmed
        /// </summary>
        public OnFoxDisarmedDelegate OnFoxDisarmed { get; set; }

        /// <summary>
        /// Delegate, called when profile settings changed from fox menu
        /// </summary>
        public OnProfileSettingsChangedDelegate OnProfileSettingsChanged { get; set; }

        /// <summary>
        /// Delegate, called when profile changed
        /// </summary>
        public OnProfileChangedDelegate OnProfileChanged { get; set; }

        #endregion

        #region View Models (kinda dirty)

        /// <summary>
        /// Main page view model (to update it's properties from other pages)
        /// </summary>
        public MainPageViewModel MainPageViewModel;

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
            OnEnteringSleepmode = null;
            OnFoxDisarmed = null;
            OnProfileChanged = null;

            AppDisplays.Push(ActiveDisplay.MainDisplay);
        }
    }
}
