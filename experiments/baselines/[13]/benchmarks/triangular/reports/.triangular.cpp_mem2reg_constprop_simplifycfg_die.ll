; ModuleID = '.triangular.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/triangular.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z10triangularPiPA100_ii(i32* %x, [100 x i32]* %A, i32 %n) #0 {
entry:
  %sub = add nsw i32 %n, -1
  %cmp4 = icmp sgt i32 %n, 0
  br i1 %cmp4, label %for.body.lr.ph, label %for.end16

for.body.lr.ph:                                   ; preds = %entry
  br label %for.body

for.body:                                         ; preds = %for.inc14, %for.body.lr.ph
  %i.05 = phi i32 [ %sub, %for.body.lr.ph ], [ %dec15, %for.inc14 ]
  %cmp31 = icmp sgt i32 %i.05, 0
  br i1 %cmp31, label %for.body4.lr.ph, label %for.inc14

for.body4.lr.ph:                                  ; preds = %for.body
  br label %for.body4

for.body4:                                        ; preds = %for.body4, %for.body4.lr.ph
  %k.02.in = phi i32 [ %i.05, %for.body4.lr.ph ], [ %k.02, %for.body4 ]
  %k.02 = add nsw i32 %k.02.in, -1
  %idxprom = sext i32 %k.02 to i64
  %idxprom5 = sext i32 %i.05 to i64
  %arrayidx6 = getelementptr inbounds [100 x i32], [100 x i32]* %A, i64 %idxprom, i64 %idxprom5
  %0 = load i32, i32* %arrayidx6, align 4
  %idxprom7 = sext i32 %i.05 to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %x, i64 %idxprom7
  %1 = load i32, i32* %arrayidx8, align 4
  %mul = mul nsw i32 %0, %1
  %idxprom9 = sext i32 %k.02 to i64
  %idxprom11 = sext i32 %n to i64
  %arrayidx12 = getelementptr inbounds [100 x i32], [100 x i32]* %A, i64 %idxprom9, i64 %idxprom11
  %2 = load i32, i32* %arrayidx12, align 4
  %sub13 = sub nsw i32 %2, %mul
  store i32 %sub13, i32* %arrayidx12, align 4
  %cmp3 = icmp sgt i32 %k.02.in, 1
  br i1 %cmp3, label %for.body4, label %for.inc14

for.inc14:                                        ; preds = %for.body4, %for.body
  %dec15 = add nsw i32 %i.05, -1
  %cmp = icmp sgt i32 %i.05, 0
  br i1 %cmp, label %for.body, label %for.end16

for.end16:                                        ; preds = %for.inc14, %entry
  %i.0.lcssa = phi i32 [ %sub, %entry ], [ %dec15, %for.inc14 ]
  %add = add nsw i32 %i.0.lcssa, %n
  ret i32 %add
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %xArray = alloca [1 x [100 x i32]], align 16
  %A = alloca [1 x [100 x [100 x i32]]], align 16
  %n = alloca [1 x i32], align 4
  br label %for.body

for.body:                                         ; preds = %for.inc22, %entry
  %0 = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 0
  store i32 100, i32* %0, align 4
  br label %for.body3

for.body3:                                        ; preds = %for.inc19, %for.body
  %x.02 = phi i32 [ 0, %for.body ], [ %inc20, %for.inc19 ]
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 1000
  %1 = zext i32 %x.02 to i64
  %2 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %xArray, i64 0, i64 0, i64 %1
  store i32 %rem, i32* %2, align 4
  br label %for.body10

for.body10:                                       ; preds = %for.body10, %for.body3
  %y.01 = phi i32 [ 0, %for.body3 ], [ %inc, %for.body10 ]
  %call11 = call i32 @rand() #3
  %rem12 = srem i32 %call11, 1000
  %3 = zext i32 %y.01 to i64
  %4 = zext i32 %x.02 to i64
  %5 = getelementptr inbounds [1 x [100 x [100 x i32]]], [1 x [100 x [100 x i32]]]* %A, i64 0, i64 0, i64 %3, i64 %4
  store i32 %rem12, i32* %5, align 4
  %inc = add nuw nsw i32 %y.01, 1
  %cmp9 = icmp ult i32 %inc, 10
  br i1 %cmp9, label %for.body10, label %for.inc19

for.inc19:                                        ; preds = %for.body10
  %inc20 = add nuw nsw i32 %x.02, 1
  %cmp2 = icmp ult i32 %inc20, 100
  br i1 %cmp2, label %for.body3, label %for.inc22

for.inc22:                                        ; preds = %for.inc19
  br i1 false, label %for.body, label %for.end24

for.end24:                                        ; preds = %for.inc22
  %arraydecay = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %xArray, i64 0, i64 0, i64 0
  %arraydecay30 = getelementptr inbounds [1 x [100 x [100 x i32]]], [1 x [100 x [100 x i32]]]* %A, i64 0, i64 0, i64 0
  %arrayidx32 = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 0
  %6 = load i32, i32* %arrayidx32, align 4
  %call33 = call i32 @_Z10triangularPiPA100_ii(i32* nonnull %arraydecay, [100 x i32]* nonnull %arraydecay30, i32 %6)
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @rand() #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
