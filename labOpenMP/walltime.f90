function walltime()
    integer, parameter:: DP = kind(0.0D0)
    real(DP) walltime
    integer::count,count_rate,count_max
    call system_clock(count,count_rate,count_max)    
    walltime=real(count,DP)/real(count_rate,DP)
end function walltime
