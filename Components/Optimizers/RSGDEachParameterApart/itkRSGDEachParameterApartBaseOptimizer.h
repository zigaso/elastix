/*=========================================================================
 *
 *  Copyright UMC Utrecht and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef itkRSGDEachParameterApartBaseOptimizer_h
#define itkRSGDEachParameterApartBaseOptimizer_h

#include "itkSingleValuedNonLinearOptimizer.h"

namespace itk
{

/**
 * \class RSGDEachParameterApartBaseOptimizer
 * \brief An optimizer based on gradient descent...
 *
 * This optimizer
 *
 * \ingroup Optimizers
 */

class RSGDEachParameterApartBaseOptimizer : public SingleValuedNonLinearOptimizer
{
public:
  /** Standard "Self" typedef. */
  typedef RSGDEachParameterApartBaseOptimizer Self;
  typedef SingleValuedNonLinearOptimizer      Superclass;
  typedef SmartPointer<Self>                  Pointer;
  typedef SmartPointer<const Self>            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RSGDEachParameterApartBaseOptimizer, SingleValuedNonLinearOptimizer);

  /** Codes of stopping conditions. */
  typedef enum
  {
    GradientMagnitudeTolerance = 1,
    StepTooSmall,
    ImageNotAvailable,
    SamplesNotAvailable,
    MaximumNumberOfIterations,
    MetricError
  } StopConditionType;

  /** Specify whether to minimize or maximize the cost function. */
  itkSetMacro(Maximize, bool);
  itkGetConstMacro(Maximize, bool);
  itkBooleanMacro(Maximize);
  bool
  GetMinimize() const
  {
    return !m_Maximize;
  }
  void
  SetMinimize(bool v)
  {
    this->SetMaximize(!v);
  }
  void
  MinimizeOn(void)
  {
    SetMaximize(false);
  }
  void
  MinimizeOff(void)
  {
    SetMaximize(true);
  }

  /** Start optimization. */
  void
  StartOptimization(void) override;

  /** Resume previously stopped optimization with current parameters.
   * \sa StopOptimization */
  void
  ResumeOptimization(void);

  /** Stop optimization.
   * \sa ResumeOptimization */
  void
  StopOptimization(void);

  /** Set/Get parameters to control the optimization process. */
  itkSetMacro(MaximumStepLength, double);
  itkSetMacro(MinimumStepLength, double);
  itkSetMacro(NumberOfIterations, unsigned long);
  itkSetMacro(GradientMagnitudeTolerance, double);
  itkGetConstMacro(MaximumStepLength, double);
  itkGetConstMacro(MinimumStepLength, double);
  itkGetConstMacro(NumberOfIterations, unsigned long);
  itkGetConstMacro(GradientMagnitudeTolerance, double);
  itkGetConstMacro(CurrentIteration, unsigned long);
  itkGetConstMacro(StopCondition, StopConditionType);
  itkGetConstMacro(Value, MeasureType);
  itkGetConstReferenceMacro(Gradient, DerivativeType);

  /** Get the array of all step lengths */
  itkGetConstReferenceMacro(CurrentStepLengths, DerivativeType);

  /** Get the current average step length */
  itkGetConstMacro(CurrentStepLength, double);

  /** Get the current GradientMagnitude */
  itkGetConstMacro(GradientMagnitude, double);

protected:
  RSGDEachParameterApartBaseOptimizer();
  ~RSGDEachParameterApartBaseOptimizer() override = default;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Advance one step following the gradient direction
   * This method verifies if a change in direction is required
   * and if a reduction in steplength is required. */
  virtual void
  AdvanceOneStep(void);

  /** Advance one step along the corrected gradient taking into
   * account the steplength represented by factor.
   * This method is invoked by AdvanceOneStep. It is expected
   * to be overrided by optimization methods in non-vector spaces
   *
   * In RSGDEachParameterApart this function does not accepts a
   * single scalar steplength factor, but an array of factors,
   * which contains the steplength for each parameter apart.
   *
   * \sa AdvanceOneStep */
  virtual void
  StepAlongGradient(const DerivativeType &, const DerivativeType &)
  {
    ExceptionObject ex;
    ex.SetLocation(__FILE__);
    ex.SetDescription("This method MUST be overloaded in derived classes");
    throw ex;
  }


private:
  RSGDEachParameterApartBaseOptimizer(const Self &) = delete;
  void
  operator=(const Self &) = delete;

  DerivativeType m_Gradient;
  DerivativeType m_PreviousGradient;

  bool        m_Stop{ false };
  bool        m_Maximize{ false };
  MeasureType m_Value{ 0.0 };
  double      m_GradientMagnitudeTolerance{ 1e-4 };
  double      m_MaximumStepLength{ 1.0 };
  double      m_MinimumStepLength{ 1e-3 };

  /** All current step lengths */
  DerivativeType m_CurrentStepLengths;
  /** The average current step length */
  double m_CurrentStepLength{ 0 };

  StopConditionType m_StopCondition{ MaximumNumberOfIterations };
  unsigned long     m_NumberOfIterations{ 100 };
  unsigned long     m_CurrentIteration{ 0 };

  double m_GradientMagnitude{ 0.0 };
};

} // end namespace itk

#endif // end #ifndef itkRSGDEachParameterApartBaseOptimizer_h
