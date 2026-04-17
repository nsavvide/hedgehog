use std::ffi::CString;
use std::os::raw::c_char;

#[repr(C)]
pub struct ParsedUrl {
    pub host: *mut c_char,
    pub path: *mut c_char,
}

/// no_mangle allows C to find function by name
// TODO: update url parameter
#[unsafe(no_mangle)]
pub extern "C" fn parse_url(_url: *const c_char) -> ParsedUrl {
    let host = "example.com";
    let path = "/index.html";

    ParsedUrl {
        host: CString::new(host).unwrap().into_raw(),
        path: CString::new(path).unwrap().into_raw(),
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn free_parsed_url(parsed_url: ParsedUrl) {
    unsafe {
        if !parsed_url.host.is_null() {
            let _ = CString::from_raw(parsed_url.host); // Reclaim ownership to free memory
        }
        if !parsed_url.path.is_null() {
            let _ = CString::from_raw(parsed_url.path); // Reclaim ownership to free memory
        }
    }
}
