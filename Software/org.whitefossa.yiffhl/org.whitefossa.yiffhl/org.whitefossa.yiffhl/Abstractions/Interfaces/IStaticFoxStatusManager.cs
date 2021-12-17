using org.whitefossa.yiffhl.Abstractions.DTOs;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetStaticFoxStatusDelegate(StaticFoxStatus status);

    public delegate void OnArmFoxDelegate();

    public interface IStaticFoxStatusManager
    {
        Task GetStaticFoxStatusAsync(OnGetStaticFoxStatusDelegate onGetStaticFoxStatus);

        Task ArmFoxAsync(OnArmFoxDelegate onArmFox);
    }
}
