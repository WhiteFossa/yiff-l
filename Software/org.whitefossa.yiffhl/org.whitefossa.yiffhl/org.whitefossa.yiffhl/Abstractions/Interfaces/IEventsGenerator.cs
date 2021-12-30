using org.whitefossa.yiffhl.Models;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnShowMatchingDisplayDelegate();

    public delegate void OnFoxArmedDelegate();

    public delegate void OnFoxDisarmedDelegate();

    /// <summary>
    /// Generates events based on fox status changes
    /// </summary>
    public interface IEventsGenerator
    {
        Task GenerateEventsAsync(MainModel model);

        void RegisterOnShowMatchingDisplayHandler(OnShowMatchingDisplayDelegate onShowMatchingDisplay);

        void RegisterOnFoxArmedHandler(OnFoxArmedDelegate onFoxArmed);

        void RegisterOnFoxDisarmedHandler(OnFoxDisarmedDelegate onFoxDisarmed);
    }
}
