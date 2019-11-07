// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @brief A header file with declaration for VehicleAttribsDetection Class
 * @file vehicle_attribs_detection.hpp
 */
#ifndef DYNAMIC_VINO_LIB__INFERENCES__VEHICLE_ATTRIBS_DETECTION_HPP_
#define DYNAMIC_VINO_LIB__INFERENCES__VEHICLE_ATTRIBS_DETECTION_HPP_
#include <memory>
#include <vector>
#include <string>
#include "dynamic_vino_lib/models/vehicle_attribs_detection_model.h"
#include "dynamic_vino_lib/engines/engine.h"
#include "dynamic_vino_lib/inferences/base_inference.h"
#include "inference_engine.hpp"
#include "opencv2/opencv.hpp"
// namespace
namespace dynamic_vino_lib
{
/**
 * @class VehicleAttribsDetectionResult
 * @brief Class for storing and processing vehicle attributes detection result.
 */
class VehicleAttribsDetectionResult : public Result
{
public:
  friend class VehicleAttribsDetection;
  explicit VehicleAttribsDetectionResult(const cv::Rect & location);
  std::string getColor() const
  {
    return color_;
  }
  std::string getType() const
  {
    return type_;
  }

private:
  std::string color_ = "";
  std::string type_ = "";
};
/**
 * @class VehicleAttribsDetection
 * @brief Class to load vehicle attributes detection model and perform detection.
 */
class VehicleAttribsDetection : public BaseInference
{
public:
  using Result = dynamic_vino_lib::VehicleAttribsDetectionResult;
  VehicleAttribsDetection();
  ~VehicleAttribsDetection() override;
  /**
   * @brief Load the vehicle attributes detection model.
   */
  void loadNetwork(std::shared_ptr<Models::VehicleAttribsDetectionModel>);
  /**
   * @brief Enqueue a frame to this class.
   * The frame will be buffered but not infered yet.
   * @param[in] frame The frame to be enqueued.
   * @param[in] input_frame_loc The location of the enqueued frame with respect
   * to the frame generated by the input device.
   * @return Whether this operation is successful.
   */
  bool enqueue(const cv::Mat &, const cv::Rect &) override;
  /**
   * @brief Start inference for all buffered frames.
   * @return Whether this operation is successful.
   */
  bool submitRequest() override;
  /**
   * @brief This function will fetch the results of the previous inference and
   * stores the results in a result buffer array. All buffered frames will be
   * cleared.
   * @return Whether the Inference object fetches a result this time
   */
  bool fetchResults() override;
  /**
   * @brief Get the length of the buffer result array.
   * @return The length of the buffer result array.
   */
  const int getResultsLength() const override;
  /**
   * @brief Get the location of result with respect
   * to the frame generated by the input device.
   * @param[in] idx The index of the result.
   */
  const dynamic_vino_lib::Result * getLocationResult(int idx) const override;
  /**
   * @brief Show the observed detection result either through image window
     or ROS topic.
   */
  const void observeOutput(const std::shared_ptr<Outputs::BaseOutput> & output,
    const std::string filter_conditions);
  /**
   * @brief Get the name of the Inference instance.
   * @return The name of the Inference instance.
   */
  const std::string getName() const override;
  const std::vector<cv::Rect> getFilteredROIs(
    const std::string filter_conditions) const override;

private:
  std::shared_ptr<Models::VehicleAttribsDetectionModel> valid_model_;
  std::vector<Result> results_;
  const std::vector<std::string> types_ = {
    "car", "van", "truck", "bus"};
  const std::vector<std::string> colors_ = {
    "white", "gray", "yellow", "red", "green", "blue", "black"};
};
}  // namespace dynamic_vino_lib
#endif  // DYNAMIC_VINO_LIB__INFERENCES__VEHICLE_ATTRIBS_DETECTION_HPP_