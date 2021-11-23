using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Models;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnFoxProfilesEnumeratedDelegate(IReadOnlyCollection<Profile> profiles);

    /// <summary>
    /// Fox profiles enumerator
    /// </summary>
    public interface IFoxProfilesEnumerator
    {
        Task EnumerateProfilesAsync(MainModel mainModel, OnFoxProfilesEnumeratedDelegate onProfilesEnumerated);
    }
}
