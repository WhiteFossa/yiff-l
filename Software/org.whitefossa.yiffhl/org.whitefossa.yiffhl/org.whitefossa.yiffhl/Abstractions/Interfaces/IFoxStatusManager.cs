using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetFoxStatus(FoxStatus status);

    public interface IFoxStatusManager
    {
        Task GetFoxStatusAsync(OnGetFoxStatus onGetFoxStatus);
    }
}
