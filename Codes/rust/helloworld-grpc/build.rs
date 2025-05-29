fn main() {
    tonic_build::compile_protos("proto/helloworld.proto").expect("Failed to compile protos");
}
