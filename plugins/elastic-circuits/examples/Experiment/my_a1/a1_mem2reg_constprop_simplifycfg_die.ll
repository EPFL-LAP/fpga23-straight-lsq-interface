; ModuleID = '_build/a1/a1_mem2reg_constprop_simplifycfg.ll'
source_filename = "a1.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z3a_1ii(i32 %a, i32 %b) #0 {
entry:
  %cmp = icmp slt i32 %a, %b
  br i1 %cmp, label %if.else, label %if.else3

if.else:                                          ; preds = %entry
  br label %if.end4

if.else3:                                         ; preds = %entry
  %mul = shl nsw i32 %a, 2
  br label %if.end4

if.end4:                                          ; preds = %if.else3, %if.else
  %c.1 = phi i32 [ 1, %if.else ], [ %mul, %if.else3 ]
  ret i32 %c.1
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
