use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use std::ptr;
use url::Url;

#[repr(C)]
pub struct ParsedUrl {
    pub host: *mut c_char,
    pub path: *mut c_char,
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn parse_url(raw_url: *const c_char) -> ParsedUrl {
    if raw_url.is_null() {
        return ParsedUrl {
            host: ptr::null_mut(),
            path: ptr::null_mut(),
        };
    }

    let c_str = unsafe { CStr::from_ptr(raw_url) };
    let url_str = match c_str.to_str() {
        Ok(s) => s,
        Err(_) => {
            return ParsedUrl {
                host: ptr::null_mut(),
                path: ptr::null_mut(),
            };
        }
    };

    match Url::parse(url_str) {
        Ok(parsed) => {
            let host_str = parsed.host_str().unwrap_or("");

            let mut path_str = parsed.path().to_string();
            if let Some(query) = parsed.query() {
                path_str.push('?');
                path_str.push_str(query);
            }

            let host_cstring = CString::new(host_str).unwrap_or_default();
            let path_cstring = CString::new(path_str).unwrap_or_default();

            ParsedUrl {
                host: host_cstring.into_raw(),
                path: path_cstring.into_raw(),
            }
        }
        Err(_) => ParsedUrl {
            host: ptr::null_mut(),
            path: ptr::null_mut(),
        },
    }
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn free_parsed_url(parsed_url_ptr: *mut ParsedUrl) {
    if parsed_url_ptr.is_null() {
        return;
    }

    let parsed_url = unsafe { &mut *parsed_url_ptr };

    if !parsed_url.host.is_null() {
        let _ = unsafe { CString::from_raw(parsed_url.host) };
        parsed_url.host = ptr::null_mut();
    }

    if !parsed_url.path.is_null() {
        let _ = unsafe { CString::from_raw(parsed_url.path) };
        parsed_url.path = ptr::null_mut();
    }
}
