using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnClockSynchronizedDelegate(bool isSuccessful);

    /// <summary>
    /// Interface to work with fox clock
    /// </summary>
    public interface IFoxClockManager
    {
        Task SynchronizeClockAsync(OnClockSynchronizedDelegate onSynchronized);
    }
}
