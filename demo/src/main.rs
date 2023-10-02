use std::ptr::null;

use blackbone_sys::bindings::*;
use blackbone_sys::types::*;

/// define a macro to make const wchat_t* from a string literal
#[allow(unused_macros)]
macro_rules! w {
    ($s:literal) => {
        $s.encode_utf16()
            .chain(Some(0))
            .collect::<Vec<_>>()
            .as_ptr()
    };
    ($s:expr) => {
        $s.encode_utf16()
            .chain(Some(0))
            .collect::<Vec<_>>()
            .as_ptr()
    };
}

unsafe extern "C" fn enum_by_name_callback(pid: DWORD, lparam: LPARAM) {
    let pid_list = &mut *(lparam as *mut Vec<DWORD>);
    pid_list.push(pid);
}

#[allow(dead_code)]
unsafe fn test_process_terminate() {
    let process = Process_Create();
    assert!(!process.is_null());

    let status = Process_Attach(process, 28512, 3451);
    assert_eq!(status, 0);

    let pid = Process_pid(process);
    println!("pid: {}", pid);
    assert_ne!(pid, 0);

    Process_Terminate(process, 0);

    let status = Process_Detach(process);
    assert_eq!(status, 0);

    Process_Destroy(process);
}

#[allow(dead_code)]
unsafe fn test_enum_process_by_name() {
    let mut pid_list = Vec::<DWORD>::new();
    Process_EnumByName(
        w!("notepad.exe"),
        Some(enum_by_name_callback),
        &mut pid_list as *mut _ as LPARAM,
    );
    println!("pid_list: {:#?}", pid_list);
}

unsafe extern "C" fn enum_handle_callback(handle_info: *mut Process_HandleInfo, _lparam: LPARAM) {
    println!(
        "typeName: {}, name: {}",
        to_string((*handle_info).typeName),
        to_string((*handle_info).name)
    );
}

unsafe fn get_process_id(name: &str) -> DWORD {
    let mut pid_list = Vec::<DWORD>::new();
    Process_EnumByName(
        w!(name),
        Some(enum_by_name_callback),
        &mut pid_list as *mut _ as LPARAM,
    );
    assert_eq!(pid_list.is_empty(), false);
    pid_list[0]
}

#[allow(dead_code)]
fn to_string(ptr: *const u16) -> String {
    unsafe {
        let len = (0..).take_while(|&i| *ptr.offset(i) != 0).count();
        let slice = std::slice::from_raw_parts(ptr, len);
        String::from_utf16_lossy(slice)
    }
}

#[allow(dead_code)]
unsafe fn test_enum_handles() {
    let process = Process_Create();
    assert!(!process.is_null());

    let pid = get_process_id("notepad.exe");
    println!("pid: {}", pid);
    let status = Process_Attach(process, pid, 2097151);
    assert_eq!(status, 0);

    Process_EnumHandles(process, Some(enum_handle_callback), 0);
}


unsafe extern "C" fn enum_process_callback(process_info: *mut Process_ProcessInfo, _lparam: LPARAM) {
    println!(
        "pid: {}, name: {}",
        (*process_info).pid,
        to_string((*process_info).imageName)
    );
}

unsafe fn test_enum_process() {
    Process_EnumByNameOrPID(0, null(), 0, Some(enum_process_callback), 0)
}

fn main() {
    unsafe {
        test_enum_process();
    }
}
