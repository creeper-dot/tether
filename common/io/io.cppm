// tether.io  主模块接口
//
// import tether.io; 之后能用到 ByteWriter/ByteReader、read_exact/write_all、
// Message/FrameWriter/FrameReader，以及 tether.util 的全部内容（这里重新导出了）。

export module tether.io;

export import tether.util;
export import :bytes;
export import :fd;
export import :framing;
