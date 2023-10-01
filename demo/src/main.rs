use blackbone_sys::bindings::*;
use blackbone_sys::types::*;

/// define a macro to make const wchat_t* from a string literal
#[allow(unused_macros)]
macro_rules! w {
    ($s:literal) => {
        $s.encode_utf16().chain(Some(0)).collect::<Vec<_>>().as_ptr()
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

unsafe fn test_enum_process_by_name() {
    let mut pid_list = Vec::<DWORD>::new();
    Process_EnumByName(w!("notepad.exe"), 
        Some(enum_by_name_callback), 
        &mut pid_list as *mut _ as LPARAM);
    println!("pid_list: {:#?}", pid_list);
}

fn main() {
    unsafe {
        test_enum_process_by_name();
    }
}