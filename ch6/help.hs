type Time = Float

-- computes n+1 CPU burst time guess from previous n actual times, alpha value, and initial guess
-- give the times list in reverse chronological order
tau :: [Time] -> Float -> Time -> Time
tau [] _ t = t
tau (ti:tis) a t = a * ti + (1 - a) * (tau tis a t)