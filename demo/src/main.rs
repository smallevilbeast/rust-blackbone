use blackbone_sys::bindings::*;

/// define a macro to make const wchat_t* from a string literal
#[allow(unused_macros)]
macro_rules! w {
    ($s:literal) => {
        $s.encode_utf16().chain(Some(0)).collect::<Vec<_>>().as_ptr()
    };
}

unsafe fn test_process() {
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

fn main() {
    unsafe {
        test_process();
    }
}