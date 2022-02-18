using System;

namespace org.whitefossa.yiffhl.Business.Helpers
{
    /// <summary>
    /// Class to work with delegates
    /// </summary>
    public static class DelegatesHelper
    {
        /// <summary>
        /// Remove all connected methods from delegate
        /// </summary>
        public static T RemoveAllMethods<T>(T delegateToClear) where T : MulticastDelegate
        {
            foreach (T d in delegateToClear.GetInvocationList())
            {
                delegateToClear = (T)Delegate.Remove(delegateToClear, d);
            }

            return delegateToClear;
        }
    }
}
