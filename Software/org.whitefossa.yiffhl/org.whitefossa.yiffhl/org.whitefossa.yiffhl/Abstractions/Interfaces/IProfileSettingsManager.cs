using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetFrequencySettingsDelegate(FrequencySettings settings);

    public delegate void OnSetFrequencySettingsDelegate();

    public delegate void OnGetCallsignSettingsDelegate(CallsignSettings settings);

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
    }
}
