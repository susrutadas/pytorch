#include <ATen/LegacyTHFunctionsCUDA.h>

#include <ATen/ATen.h>
#include <ATen/Utils.h>
#include <ATen/NamedTensorUtils.h>
#include <ATen/CUDAGeneratorImpl.h>
#include <ATen/ExpandUtils.h>
#include <THC/THC.h>
#include <THC/THCTensor.hpp>
#include <THCUNN/THCUNN.h>
#undef THNN_
#undef THCIndexTensor_
#include <ATen/DeviceGuard.h>
#include <ATen/cuda/ATenCUDAGeneral.h>
#include <ATen/cuda/CUDADevice.h>
#include <ATen/cuda/CUDAContext.h>

namespace at {
namespace native {
namespace legacy {
namespace cuda {

namespace {
  ScalarType infer_scalar_type(const Tensor & t) {
    return t.scalar_type();
  }
  ScalarType infer_scalar_type(const TensorList & tl) {
    TORCH_CHECK(tl.size() > 0, "expected a non-empty list of Tensors");
    return tl[0].scalar_type();
  }

  TensorOptions options(ScalarType s) {
    return TensorOptions().dtype(s)
                          .device(DeviceType::CUDA)
                          .layout(kStrided);
  }

  Allocator* allocator() {
    return at::cuda::getCUDADeviceAllocator();
  }
}

std::tuple<Tensor &,Tensor &> _th_gels_out(const Tensor & self, const Tensor & A, Tensor & res1, Tensor & res2) {
    TORCH_WARN_ONCE(
      "torch.lstsq is deprecated in favor of torch.linalg.lstsq and will be removed in a future PyTorch release.\n",
      "torch.linalg.lstsq has reversed arguments and does not return the QR decomposition in "
      "the returned tuple (although it returns other information about the problem).\n",
      "To get the qr decomposition consider using torch.linalg.qr.\n",
      "The returned solution in torch.lstsq stored the residuals of the solution in the ",
      "last m - n columns of the returned value whenever m > n. In torch.linalg.lstsq, the ",
      "residuals in the field 'residuals' of the returned named tuple.\n",
      "The unpacking of the solution, as in\n",
      "X, _ = torch.lstsq(B, A).solution[:A.size(1)]\n",
      "should be replaced with\n",
      "X = torch.linalg.lstsq(A, B).solution"
    );
    // DeviceGuard omitted
    auto dispatch_scalar_type = infer_scalar_type(self);

    switch (dispatch_scalar_type) {
        case ScalarType::Double: {
            auto res1_ = checked_dense_tensor_unwrap(res1, "res1", 0, "_th_gels_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto res2_ = checked_dense_tensor_unwrap(res2, "res2", 0, "_th_gels_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_gels_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto A_ = checked_dense_tensor_unwrap(A, "A", 2, "_th_gels_out", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaDoubleTensor_gels(globalContext().getTHCState(), res1_, res2_, self_, A_);
            break;
        }
        case ScalarType::Float: {
            auto res1_ = checked_dense_tensor_unwrap(res1, "res1", 0, "_th_gels_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto res2_ = checked_dense_tensor_unwrap(res2, "res2", 0, "_th_gels_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_gels_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto A_ = checked_dense_tensor_unwrap(A, "A", 2, "_th_gels_out", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaTensor_gels(globalContext().getTHCState(), res1_, res2_, self_, A_);
            break;
        }
        default:
            AT_ERROR("_th_gels_out not supported on CUDAType for ", dispatch_scalar_type);
    }
    return std::tuple<Tensor &, Tensor &>(res1, res2);
}
std::tuple<Tensor,Tensor> _th_gels(const Tensor & self, const Tensor & A) {
    TORCH_WARN_ONCE(
      "torch.lstsq is deprecated in favor of torch.linalg.lstsq and will be removed in a future PyTorch release.\n",
      "torch.linalg.lstsq has reversed arguments and does not return the QR decomposition in "
      "the returned tuple (although it returns other information about the problem).\n",
      "To get the qr decomposition consider using torch.linalg.qr.\n",
      "The returned solution in torch.lstsq stored the residuals of the solution in the ",
      "last m - n columns of the returned value whenever m > n. In torch.linalg.lstsq, the ",
      "residuals in the field 'residuals' of the returned named tuple.\n",
      "The unpacking of the solution, as in\n",
      "X, _ = torch.lstsq(B, A).solution[:A.size(1)]\n",
      "should be replaced with\n",
      "X = torch.linalg.lstsq(A, B).solution"
    );
    // DeviceGuard omitted
    auto dispatch_scalar_type = infer_scalar_type(self);
    auto res1_ = c10::make_intrusive<TensorImpl, UndefinedTensorImpl>(c10::Storage(c10::Storage::use_byte_size_t(), 0, allocator(), true),DispatchKey::CUDA, scalarTypeToTypeMeta(dispatch_scalar_type)).release();
    auto res1 = Tensor(c10::intrusive_ptr<TensorImpl, UndefinedTensorImpl>::reclaim(res1_));
    auto res2_ = c10::make_intrusive<TensorImpl, UndefinedTensorImpl>(c10::Storage(c10::Storage::use_byte_size_t(), 0, allocator(), true),DispatchKey::CUDA, scalarTypeToTypeMeta(dispatch_scalar_type)).release();
    auto res2 = Tensor(c10::intrusive_ptr<TensorImpl, UndefinedTensorImpl>::reclaim(res2_));
    switch (dispatch_scalar_type) {
        case ScalarType::Double: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_gels", false, DeviceType::CUDA, dispatch_scalar_type);
            auto A_ = checked_dense_tensor_unwrap(A, "A", 2, "_th_gels", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaDoubleTensor_gels(globalContext().getTHCState(), res1_, res2_, self_, A_);
            break;
        }
        case ScalarType::Float: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_gels", false, DeviceType::CUDA, dispatch_scalar_type);
            auto A_ = checked_dense_tensor_unwrap(A, "A", 2, "_th_gels", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaTensor_gels(globalContext().getTHCState(), res1_, res2_, self_, A_);
            break;
        }
        default:
            AT_ERROR("_th_gels not supported on CUDAType for ", dispatch_scalar_type);
    }
    return std::tuple<Tensor, Tensor>(res1, res2);
}
Tensor & _th_copy_ignoring_overlaps_(Tensor & self, const Tensor & src) {
    // DeviceGuard omitted
    auto dispatch_scalar_type = infer_scalar_type(self);

    switch (dispatch_scalar_type) {
        case ScalarType::Byte: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            auto src_ = checked_dense_tensor_unwrap(src, "src", 2, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaByteTensor_copyIgnoringOverlaps(globalContext().getTHCState(), self_, src_);
            break;
        }
        case ScalarType::Char: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            auto src_ = checked_dense_tensor_unwrap(src, "src", 2, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaCharTensor_copyIgnoringOverlaps(globalContext().getTHCState(), self_, src_);
            break;
        }
        case ScalarType::Double: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            auto src_ = checked_dense_tensor_unwrap(src, "src", 2, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaDoubleTensor_copyIgnoringOverlaps(globalContext().getTHCState(), self_, src_);
            break;
        }
        case ScalarType::Float: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            auto src_ = checked_dense_tensor_unwrap(src, "src", 2, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaTensor_copyIgnoringOverlaps(globalContext().getTHCState(), self_, src_);
            break;
        }
        case ScalarType::Int: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            auto src_ = checked_dense_tensor_unwrap(src, "src", 2, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaIntTensor_copyIgnoringOverlaps(globalContext().getTHCState(), self_, src_);
            break;
        }
        case ScalarType::Long: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            auto src_ = checked_dense_tensor_unwrap(src, "src", 2, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaLongTensor_copyIgnoringOverlaps(globalContext().getTHCState(), self_, src_);
            break;
        }
        case ScalarType::Short: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            auto src_ = checked_dense_tensor_unwrap(src, "src", 2, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaShortTensor_copyIgnoringOverlaps(globalContext().getTHCState(), self_, src_);
            break;
        }
        case ScalarType::Half: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            auto src_ = checked_dense_tensor_unwrap(src, "src", 2, "_th_copy_ignoring_overlaps_", false, DeviceType::CUDA, dispatch_scalar_type);
            THCudaHalfTensor_copyIgnoringOverlaps(globalContext().getTHCState(), self_, src_);
            break;
        }
        default:
            AT_ERROR("_th_copy_ignoring_overlaps_ not supported on CUDAType for ", dispatch_scalar_type);
    }
    return self;
}
std::tuple<Tensor &,Tensor &,Tensor &> _thnn_conv2d_forward_out(const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const c10::optional<Tensor>& bias_opt, IntArrayRef stride, IntArrayRef padding, Tensor & output, Tensor & columns, Tensor & ones) {
  // See [Note: hacky wrapper removal for optional tensor]
  c10::MaybeOwned<Tensor> bias_maybe_owned = at::borrow_from_optional_tensor(bias_opt);
  const Tensor& bias = *bias_maybe_owned;

    const OptionalDeviceGuard device_guard(device_of(self));
    auto dispatch_scalar_type = infer_scalar_type(self);

    switch (dispatch_scalar_type) {
        case ScalarType::Double: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 2, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 3);
            auto bias_ = checked_dense_tensor_unwrap(bias, "bias", 4, "_thnn_conv2d_forward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto output_ = checked_dense_tensor_unwrap(output, "output", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            THNN_CudaDoubleSpatialConvolutionMM_updateOutput(globalContext().getTHCState(), self_, output_, weight_, bias_ ? bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            break;
        }
        case ScalarType::Float: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 2, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 3);
            auto bias_ = checked_dense_tensor_unwrap(bias, "bias", 4, "_thnn_conv2d_forward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto output_ = checked_dense_tensor_unwrap(output, "output", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            THNN_CudaSpatialConvolutionMM_updateOutput(globalContext().getTHCState(), self_, output_, weight_, bias_ ? bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            break;
        }
        case ScalarType::Half: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 2, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 3);
            auto bias_ = checked_dense_tensor_unwrap(bias, "bias", 4, "_thnn_conv2d_forward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto output_ = checked_dense_tensor_unwrap(output, "output", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            THNN_CudaHalfSpatialConvolutionMM_updateOutput(globalContext().getTHCState(), self_, output_, weight_, bias_ ? bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            break;
        }
        case ScalarType::BFloat16: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 2, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 3);
            auto bias_ = checked_dense_tensor_unwrap(bias, "bias", 4, "_thnn_conv2d_forward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto output_ = checked_dense_tensor_unwrap(output, "output", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 6, "_thnn_conv2d_forward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            THNN_CudaBFloat16SpatialConvolutionMM_updateOutput(globalContext().getTHCState(), self_, output_, weight_, bias_ ? bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            break;
        }
        default:
            AT_ERROR("_thnn_conv2d_forward_out not supported on CUDAType for ", dispatch_scalar_type);
    }
    return std::tuple<Tensor &, Tensor &, Tensor &>(output, columns, ones);
}
std::tuple<Tensor,Tensor,Tensor> _thnn_conv2d_forward(const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, const c10::optional<Tensor>& bias_opt, IntArrayRef stride, IntArrayRef padding) {
  // See [Note: hacky wrapper removal for optional tensor]
  c10::MaybeOwned<Tensor> bias_maybe_owned = at::borrow_from_optional_tensor(bias_opt);
  const Tensor& bias = *bias_maybe_owned;

    const OptionalDeviceGuard device_guard(device_of(self));
    auto dispatch_scalar_type = infer_scalar_type(self);
    auto output_ = c10::make_intrusive<TensorImpl, UndefinedTensorImpl>(c10::Storage(c10::Storage::use_byte_size_t(), 0, allocator(), true),DispatchKey::CUDA, scalarTypeToTypeMeta(dispatch_scalar_type)).release();
    auto output = Tensor(c10::intrusive_ptr<TensorImpl, UndefinedTensorImpl>::reclaim(output_));
    auto columns_ = c10::make_intrusive<TensorImpl, UndefinedTensorImpl>(c10::Storage(c10::Storage::use_byte_size_t(), 0, allocator(), true),DispatchKey::CUDA, scalarTypeToTypeMeta(dispatch_scalar_type)).release();
    auto columns = Tensor(c10::intrusive_ptr<TensorImpl, UndefinedTensorImpl>::reclaim(columns_));
    auto ones_ = c10::make_intrusive<TensorImpl, UndefinedTensorImpl>(c10::Storage(c10::Storage::use_byte_size_t(), 0, allocator(), true),DispatchKey::CUDA, scalarTypeToTypeMeta(dispatch_scalar_type)).release();
    auto ones = Tensor(c10::intrusive_ptr<TensorImpl, UndefinedTensorImpl>::reclaim(ones_));
    switch (dispatch_scalar_type) {
        case ScalarType::Double: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_thnn_conv2d_forward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 2, "_thnn_conv2d_forward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 3);
            auto bias_ = checked_dense_tensor_unwrap(bias, "bias", 4, "_thnn_conv2d_forward", true, DeviceType::CUDA, dispatch_scalar_type);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            THNN_CudaDoubleSpatialConvolutionMM_updateOutput(globalContext().getTHCState(), self_, output_, weight_, bias_ ? bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            break;
        }
        case ScalarType::Float: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_thnn_conv2d_forward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 2, "_thnn_conv2d_forward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 3);
            auto bias_ = checked_dense_tensor_unwrap(bias, "bias", 4, "_thnn_conv2d_forward", true, DeviceType::CUDA, dispatch_scalar_type);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            THNN_CudaSpatialConvolutionMM_updateOutput(globalContext().getTHCState(), self_, output_, weight_, bias_ ? bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            break;
        }
        case ScalarType::Half: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_thnn_conv2d_forward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 2, "_thnn_conv2d_forward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 3);
            auto bias_ = checked_dense_tensor_unwrap(bias, "bias", 4, "_thnn_conv2d_forward", true, DeviceType::CUDA, dispatch_scalar_type);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            THNN_CudaHalfSpatialConvolutionMM_updateOutput(globalContext().getTHCState(), self_, output_, weight_, bias_ ? bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            break;
        }
        case ScalarType::BFloat16: {
            auto self_ = checked_dense_tensor_unwrap(self, "self", 1, "_thnn_conv2d_forward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 2, "_thnn_conv2d_forward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 3);
            auto bias_ = checked_dense_tensor_unwrap(bias, "bias", 4, "_thnn_conv2d_forward", true, DeviceType::CUDA, dispatch_scalar_type);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            THNN_CudaBFloat16SpatialConvolutionMM_updateOutput(globalContext().getTHCState(), self_, output_, weight_, bias_ ? bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            break;
        }
        default:
            AT_ERROR("_thnn_conv2d_forward not supported on CUDAType for ", dispatch_scalar_type);
    }
    return std::tuple<Tensor, Tensor, Tensor>(output, columns, ones);
}
std::tuple<Tensor &,Tensor &,Tensor &> _thnn_conv2d_backward_out(Tensor & grad_input, Tensor & grad_weight, Tensor & grad_bias, const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, const Tensor & columns, const Tensor & ones) {
    const OptionalDeviceGuard device_guard(device_of(self));
    auto dispatch_scalar_type = infer_scalar_type(self);

    switch (dispatch_scalar_type) {
        case ScalarType::Double: {
            auto grad_output_ = checked_dense_tensor_unwrap(grad_output, "grad_output", 1, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 2, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 3, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 4);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 7, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 8, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_input_ = checked_dense_tensor_unwrap(grad_input, "grad_input", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_weight_ = checked_dense_tensor_unwrap(grad_weight, "grad_weight", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_bias_ = checked_dense_tensor_unwrap(grad_bias, "grad_bias", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            if (grad_input_) THNN_CudaDoubleSpatialConvolutionMM_updateGradInput(globalContext().getTHCState(), self_, grad_output_, grad_input_ ? grad_input_ : NULL, weight_, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            if (grad_weight_ || grad_bias_) THNN_CudaDoubleSpatialConvolutionMM_accGradParameters(globalContext().getTHCState(), self_, grad_output_, grad_weight_ ? grad_weight_ : NULL, grad_bias_ ? grad_bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0], 1);
            break;
        }
        case ScalarType::Float: {
            auto grad_output_ = checked_dense_tensor_unwrap(grad_output, "grad_output", 1, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 2, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 3, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 4);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 7, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 8, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_input_ = checked_dense_tensor_unwrap(grad_input, "grad_input", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_weight_ = checked_dense_tensor_unwrap(grad_weight, "grad_weight", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_bias_ = checked_dense_tensor_unwrap(grad_bias, "grad_bias", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            if (grad_input_) THNN_CudaSpatialConvolutionMM_updateGradInput(globalContext().getTHCState(), self_, grad_output_, grad_input_ ? grad_input_ : NULL, weight_, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            if (grad_weight_ || grad_bias_) THNN_CudaSpatialConvolutionMM_accGradParameters(globalContext().getTHCState(), self_, grad_output_, grad_weight_ ? grad_weight_ : NULL, grad_bias_ ? grad_bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0], 1);
            break;
        }
        case ScalarType::Half: {
            auto grad_output_ = checked_dense_tensor_unwrap(grad_output, "grad_output", 1, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 2, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 3, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 4);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 7, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 8, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_input_ = checked_dense_tensor_unwrap(grad_input, "grad_input", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_weight_ = checked_dense_tensor_unwrap(grad_weight, "grad_weight", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_bias_ = checked_dense_tensor_unwrap(grad_bias, "grad_bias", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            if (grad_input_) THNN_CudaHalfSpatialConvolutionMM_updateGradInput(globalContext().getTHCState(), self_, grad_output_, grad_input_ ? grad_input_ : NULL, weight_, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            if (grad_weight_ || grad_bias_) THNN_CudaHalfSpatialConvolutionMM_accGradParameters(globalContext().getTHCState(), self_, grad_output_, grad_weight_ ? grad_weight_ : NULL, grad_bias_ ? grad_bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0], 1);
            break;
        }
        case ScalarType::BFloat16: {
            auto grad_output_ = checked_dense_tensor_unwrap(grad_output, "grad_output", 1, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 2, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 3, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 4);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 7, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 8, "_thnn_conv2d_backward_out", false, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_input_ = checked_dense_tensor_unwrap(grad_input, "grad_input", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_weight_ = checked_dense_tensor_unwrap(grad_weight, "grad_weight", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            auto grad_bias_ = checked_dense_tensor_unwrap(grad_bias, "grad_bias", 8, "_thnn_conv2d_backward_out", true, DeviceType::CUDA, dispatch_scalar_type);
            if (grad_input_) THNN_CudaBFloat16SpatialConvolutionMM_updateGradInput(globalContext().getTHCState(), self_, grad_output_, grad_input_ ? grad_input_ : NULL, weight_, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            if (grad_weight_ || grad_bias_) THNN_CudaBFloat16SpatialConvolutionMM_accGradParameters(globalContext().getTHCState(), self_, grad_output_, grad_weight_ ? grad_weight_ : NULL, grad_bias_ ? grad_bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0], 1);
            break;
        }
        default:
            AT_ERROR("_thnn_conv2d_backward_out not supported on CUDAType for ", dispatch_scalar_type);
    }
    return std::tuple<Tensor &, Tensor &, Tensor &>(grad_input, grad_weight, grad_bias);
}
std::tuple<Tensor,Tensor,Tensor> _thnn_conv2d_backward(const Tensor & grad_output, const Tensor & self, const Tensor & weight, IntArrayRef kernel_size, IntArrayRef stride, IntArrayRef padding, const Tensor & columns, const Tensor & ones, std::array<bool,3> output_mask) {
    const OptionalDeviceGuard device_guard(device_of(self));
    auto dispatch_scalar_type = infer_scalar_type(self);
    auto grad_input_ = output_mask[0] ? c10::make_intrusive<TensorImpl, UndefinedTensorImpl>(c10::Storage(c10::Storage::use_byte_size_t(), 0, allocator(), true),DispatchKey::CUDA, scalarTypeToTypeMeta(dispatch_scalar_type)).release() : nullptr;
    auto grad_input = Tensor(c10::intrusive_ptr<TensorImpl, UndefinedTensorImpl>::reclaim(grad_input_ == nullptr ? (TensorImpl*)UndefinedTensorImpl::singleton() : (TensorImpl*)grad_input_));
    auto grad_weight_ = output_mask[1] ? c10::make_intrusive<TensorImpl, UndefinedTensorImpl>(c10::Storage(c10::Storage::use_byte_size_t(), 0, allocator(), true),DispatchKey::CUDA, scalarTypeToTypeMeta(dispatch_scalar_type)).release() : nullptr;
    auto grad_weight = Tensor(c10::intrusive_ptr<TensorImpl, UndefinedTensorImpl>::reclaim(grad_weight_ == nullptr ? (TensorImpl*)UndefinedTensorImpl::singleton() : (TensorImpl*)grad_weight_));
    auto grad_bias_ = output_mask[2] ? c10::make_intrusive<TensorImpl, UndefinedTensorImpl>(c10::Storage(c10::Storage::use_byte_size_t(), 0, allocator(), true),DispatchKey::CUDA, scalarTypeToTypeMeta(dispatch_scalar_type)).release() : nullptr;
    auto grad_bias = Tensor(c10::intrusive_ptr<TensorImpl, UndefinedTensorImpl>::reclaim(grad_bias_ == nullptr ? (TensorImpl*)UndefinedTensorImpl::singleton() : (TensorImpl*)grad_bias_));
    switch (dispatch_scalar_type) {
        case ScalarType::Double: {
            auto grad_output_ = checked_dense_tensor_unwrap(grad_output, "grad_output", 1, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 2, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 3, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 4);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 7, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 8, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            if (grad_input_) THNN_CudaDoubleSpatialConvolutionMM_updateGradInput(globalContext().getTHCState(), self_, grad_output_, grad_input_ ? grad_input_ : NULL, weight_, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            if (grad_weight_ || grad_bias_) THNN_CudaDoubleSpatialConvolutionMM_accGradParameters(globalContext().getTHCState(), self_, grad_output_, grad_weight_ ? grad_weight_ : NULL, grad_bias_ ? grad_bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0], 1);
            break;
        }
        case ScalarType::Float: {
            auto grad_output_ = checked_dense_tensor_unwrap(grad_output, "grad_output", 1, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 2, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 3, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 4);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 7, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 8, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            if (grad_input_) THNN_CudaSpatialConvolutionMM_updateGradInput(globalContext().getTHCState(), self_, grad_output_, grad_input_ ? grad_input_ : NULL, weight_, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            if (grad_weight_ || grad_bias_) THNN_CudaSpatialConvolutionMM_accGradParameters(globalContext().getTHCState(), self_, grad_output_, grad_weight_ ? grad_weight_ : NULL, grad_bias_ ? grad_bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0], 1);
            break;
        }
        case ScalarType::Half: {
            auto grad_output_ = checked_dense_tensor_unwrap(grad_output, "grad_output", 1, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 2, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 3, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 4);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 7, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 8, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            if (grad_input_) THNN_CudaHalfSpatialConvolutionMM_updateGradInput(globalContext().getTHCState(), self_, grad_output_, grad_input_ ? grad_input_ : NULL, weight_, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            if (grad_weight_ || grad_bias_) THNN_CudaHalfSpatialConvolutionMM_accGradParameters(globalContext().getTHCState(), self_, grad_output_, grad_weight_ ? grad_weight_ : NULL, grad_bias_ ? grad_bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0], 1);
            break;
        }
        case ScalarType::BFloat16: {
            auto grad_output_ = checked_dense_tensor_unwrap(grad_output, "grad_output", 1, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto self_ = checked_dense_tensor_unwrap(self, "self", 2, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto weight_ = checked_dense_tensor_unwrap(weight, "weight", 3, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto kernel_size_ = check_intlist<2>(kernel_size, "kernel_size", 4);
            auto stride_ = check_intlist<2>(stride, "stride", 5);
            auto padding_ = check_intlist<2>(padding, "padding", 6);
            auto columns_ = checked_dense_tensor_unwrap(columns, "columns", 7, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            auto ones_ = checked_dense_tensor_unwrap(ones, "ones", 8, "_thnn_conv2d_backward", false, DeviceType::CUDA, dispatch_scalar_type);
            if (grad_input_) THNN_CudaBFloat16SpatialConvolutionMM_updateGradInput(globalContext().getTHCState(), self_, grad_output_, grad_input_ ? grad_input_ : NULL, weight_, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0]);
            if (grad_weight_ || grad_bias_) THNN_CudaBFloat16SpatialConvolutionMM_accGradParameters(globalContext().getTHCState(), self_, grad_output_, grad_weight_ ? grad_weight_ : NULL, grad_bias_ ? grad_bias_ : NULL, columns_, ones_, kernel_size_[1], kernel_size_[0], stride_[1], stride_[0], padding_[1], padding_[0], 1);
            break;
        }
        default:
            AT_ERROR("_thnn_conv2d_backward not supported on CUDAType for ", dispatch_scalar_type);
    }
    return std::tuple<Tensor, Tensor, Tensor>(grad_input, grad_weight, grad_bias);
}

} // namespace th
} // namespace legacy
} // namespace native
} // namespace at
