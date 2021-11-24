using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnProfileAddedDelegate();

    public delegate void OnGetCurrentProfileIdDelegate(int profileId);

    public delegate void OnProfileSwitchedDelegate();

    public delegate void OnProfileRenamedDelegate();

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

        Task SwitchProfileAsync(int profileId, OnProfileSwitchedDelegate onProfileSwitched);

        Task RenameCurrentProfileAsync(string newName, OnProfileRenamedDelegate onProfileRenamed);
    }
}
