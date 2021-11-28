using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetFrequencySettings(FrequencySettings settings);

    public delegate void OnSetFrequencySettings();

    /// <summary>
    /// Interface to work with profile settings
    /// </summary>
    public interface IProfileSettingsManager
    {
        Task LoadFrequencySettingsAsync(OnGetFrequencySettings onGetFrequencySettings);

        Task SetFrequencySettingsAsync(FrequencySettings settings, OnSetFrequencySettings onSetFrequencySettings);
    }
}
