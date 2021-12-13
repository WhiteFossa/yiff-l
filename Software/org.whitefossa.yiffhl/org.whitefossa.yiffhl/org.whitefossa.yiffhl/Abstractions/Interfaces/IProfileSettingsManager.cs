﻿using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetFrequencySettingsDelegate(FrequencySettings settings);

    public delegate void OnSetFrequencySettingsDelegate();

    public delegate void OnGetCallsignSettingsDelegate(CallsignSettings settings);

    public delegate void OnSetSpeedDelegate();

    public delegate void OnSetCallsignDelegate();

    public delegate void OnGetCycleSettingsDelegate(CycleSettings settings);

    public delegate void OnSetCycleSettingsDelegate();

    public delegate void OnGetRunTimesSettingsDelegate(RunTimesSettings settings);

    public delegate void OnSetRunTimesSettingsDelegate();

    public delegate void OnGetPowerSettingsDelegate(PowerSettings settings);

    public delegate void OnSetPowerSettingsDelegate();

    /// <summary>
    /// Interface to work with profile settings
    /// </summary>
    public interface IProfileSettingsManager
    {
        Task LoadFrequencySettingsAsync(OnGetFrequencySettingsDelegate onGetFrequencySettings);

        Task SetFrequencySettingsAsync(FrequencySettings settings, OnSetFrequencySettingsDelegate onSetFrequencySettings);

        /// <summary>
        /// Returns list of possible fox callsigns
        /// </summary>
        Task<IReadOnlyCollection<Callsign>> GetCallsignsAsync();

        Task LoadCallsignSettingsAsync(OnGetCallsignSettingsDelegate onGetCallsingSettings);

        Task SetSpeedAsync(bool isFast, OnSetSpeedDelegate onSetSpeed);

        Task SetCallsingAsync(Callsign callsign, OnSetCallsignDelegate onSetCallsign);

        Task LoadCycleSettingsAsync(OnGetCycleSettingsDelegate onGetCycleSettings);

        Task SetCycleSettingsAsync(CycleSettings settings, OnSetCycleSettingsDelegate onSetCycleSettings);

        Task LoadRunTimesSettinesAsync(OnGetRunTimesSettingsDelegate onGetRunTimesSettings);

        Task SetRunTimesSettingsAsync(RunTimesSettings settings, OnSetRunTimesSettingsDelegate onSetRunTimesSettines);

        Task LoadPowerSettingsAsync(OnGetPowerSettingsDelegate onGetPowerSettings);

        Task SetPowerSettingsAsync(PowerSettings settings, OnSetPowerSettingsDelegate onSetPowerSettings);
    }
}
