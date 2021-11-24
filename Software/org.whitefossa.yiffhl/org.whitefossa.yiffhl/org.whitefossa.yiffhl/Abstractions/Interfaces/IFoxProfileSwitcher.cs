using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnProfileSwitchedDelegate();

    /// <summary>
    /// Interface to switch current for profile
    /// </summary>
    public interface IFoxProfileSwitcher
    {
        Task SwitchProfileAsync(int profileId, OnProfileSwitchedDelegate onProfileSwitchedDelegate);
    }
}
