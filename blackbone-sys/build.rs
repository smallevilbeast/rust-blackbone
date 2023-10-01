use cc::windows_registry::find_tool;
use std::env;
use std::path::PathBuf;

#[allow(unused_macros)]
macro_rules! p {
    ($($tokens: tt)*) => {
        println!("cargo:warning={}", format!($($tokens)*))
    }
}

fn get_blackbone_folder() -> PathBuf {
    env::current_dir()
        .expect("Failed to get current directory")
        .join("Blackbone")
}

fn get_blackbone4c_folder() -> PathBuf {
    env::current_dir()
        .expect("Failed to get current directory")
        .join("Blackbone4C")
}

fn is_debug_build() -> bool {
    env::var("PROFILE").unwrap() == "debug"
}

fn is_64bit_build() -> bool {
    env::var("CARGO_CFG_TARGET_POINTER_WIDTH").unwrap() == "64"
}

fn build_and_link(vsproject_dir: &PathBuf, vsproject_name: &str, lib_name: &str) {
    let msbuild = find_tool("msvc", "msbuild").unwrap();

    let configuration = if is_debug_build() { "Debug" } else { "Release" };

    let platform = if is_64bit_build() { "x64" } else { "Win32" };

    // Build blackbone
    msbuild
        .to_command()
        .current_dir(vsproject_dir)
        .args(&[
            vsproject_name,
            &format!("/p:Configuration={}", configuration),
            &format!("/p:Platform={}", platform),
        ])
        .status()
        .unwrap();

    let build_lib_dir = vsproject_dir
        .join("build")
        .join(platform)
        .join(configuration);

    // Tell cargo to tell rustc to link the vc++ library.
    println!("cargo:rustc-link-search=native={}", build_lib_dir.display());

    // when use `cargo:rustc-link-lib=statuc={}` is not working, why?
    println!("cargo:rustc-link-lib={}", lib_name);
}

/// generate bindings
#[allow(dead_code)]
fn generate_bindings() {
    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .allowlist_file(".*/Blackbone4C/.*/[^/]+\\.h")
        .allowlist_recursively(false)
        .raw_line("use crate::types::*;")
        .layout_tests(false)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate()
        .expect("Failed to generate bindings");

    //let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    let out_path = PathBuf::from("src");
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Failed to write bindings");
}

fn main() {
    let blackbone4c_dir = get_blackbone4c_folder();
    build_and_link(&blackbone4c_dir, "Blackbone4C.sln", "Blackbone4C");

    let blackbone_dir = get_blackbone_folder();
    build_and_link(&blackbone_dir, "Blackbone.sln", "Blackbone");

}
