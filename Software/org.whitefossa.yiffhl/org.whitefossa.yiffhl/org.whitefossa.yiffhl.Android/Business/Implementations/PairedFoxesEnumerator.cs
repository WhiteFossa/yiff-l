using org.whitefossa.yiffhl.Abstractions.DTOs;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Droid.Business.Implementations
{
    /// <summary>
    /// Paired foxes enumerator
    /// </summary>
    internal class PairedFoxesEnumerator : IPairedFoxesEnumerator
    {
        public async Task<IReadOnlyCollection<PairedFoxDTO>> EnumerateAsync()
        {
            var result = new List<PairedFoxDTO>();

            result.Add(new PairedFoxDTO("Yiffy foxy", "11:22:33:44:55"));
            result.Add(new PairedFoxDTO("Yerfy foxy", "22:22:33:44:55"));
            result.Add(new PairedFoxDTO("Yuffy foxy", "33:22:33:44:55"));

            return result;
        }
    }
}