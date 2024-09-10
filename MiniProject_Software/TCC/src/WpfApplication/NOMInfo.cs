using System;
using System.Runtime.InteropServices;

namespace WpfApplication
{
    //NOM Data 구조체 정의
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode, Pack = 1)]
    public struct NOMInfo
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 1024)]
        public string MsgName;
        public uint MsgID;
        public int MsgLen;
    }
}
