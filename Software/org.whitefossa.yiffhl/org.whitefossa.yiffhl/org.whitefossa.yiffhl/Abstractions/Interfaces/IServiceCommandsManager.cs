using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetLastErrorCodeResultDelegate(uint lastErrorCode);

    public interface IServiceCommandsManager
    {
        Task GetLastErrorCode(OnGetLastErrorCodeResultDelegate onGetLastErrorCode);
    }
}
