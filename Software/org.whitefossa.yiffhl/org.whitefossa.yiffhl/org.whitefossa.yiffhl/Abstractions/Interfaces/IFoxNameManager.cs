using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetNameDelegate(string name);
    public delegate void OnSetNameDelegate(bool isSuccessful);

    /// <summary>
    /// Interface to work with fox name
    /// </summary>
    public interface IFoxNameManager
    {
        Task GetNameAsync(OnGetNameDelegate onGetName);

        Task SetNameAsync(string name, OnSetNameDelegate onSetName);
    }
}
