using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnProfileAddedDelegate();

    public delegate void OnGetCurrentProfileIdDelegate(int profileId);

    /// <summary>
    /// Interface to work with fox profiles (except enumeration)
    /// </summary>
    public interface IFoxProfilesManager
    {
        /// <summary>
        /// Add profile to a fox and rename it to given name.
        /// WARNING: Current profile will be switched to newly added
        /// </summary>
        Task AddProfileAsync(string newProfileName, OnProfileAddedDelegate onProfileAdded);

        Task GetCurrentProfileId(OnGetCurrentProfileIdDelegate onGetCurrentProfileId);
    }
}
