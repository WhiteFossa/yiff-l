using org.whitefossa.yiffhl.Models;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnFoxProfileAddedDelegate();

    /// <summary>
    /// Interface to add profile to a fox
    /// </summary>
    public interface IFoxProfileAdder
    {
        /// <summary>
        /// Add profile to a fox and rename it to given name.
        /// WARNING: Current profile will be switched to newly added
        /// </summary>
        Task AddProfileAsync(MainModel mainModel, string newProfileName, OnFoxProfileAddedDelegate onFoxProfileAdded);
    }
}
