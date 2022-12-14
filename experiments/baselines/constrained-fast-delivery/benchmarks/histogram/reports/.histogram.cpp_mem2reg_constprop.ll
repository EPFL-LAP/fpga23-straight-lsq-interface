; ModuleID = '.histogram.cpp_mem2reg.ll'
source_filename = "src/histogram.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z9histogramPiS_S_i(i32* %feature, i32* %weight, i32* %hist, i32 %n) #0 {
entry:
  %cmp1 = icmp slt i32 0, %n
  br i1 %cmp1, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %entry
  br label %for.body

for.body:                                         ; preds = %for.body.lr.ph, %for.inc
  %i.02 = phi i32 [ 0, %for.body.lr.ph ], [ %inc, %for.inc ]
  %idxprom = sext i32 %i.02 to i64
  %arrayidx = getelementptr inbounds i32, i32* %feature, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %i.02 to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %weight, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4
  %idxprom3 = sext i32 %0 to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %hist, i64 %idxprom3
  %2 = load i32, i32* %arrayidx4, align 4
  %add = add nsw i32 %2, %1
  %idxprom5 = sext i32 %0 to i64
  %arrayidx6 = getelementptr inbounds i32, i32* %hist, i64 %idxprom5
  store i32 %add, i32* %arrayidx6, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %inc = add nsw i32 %i.02, 1
  %cmp = icmp slt i32 %inc, %n
  br i1 %cmp, label %for.body, label %for.cond.for.end_crit_edge

for.cond.for.end_crit_edge:                       ; preds = %for.inc
  %split = phi i32 [ %inc, %for.inc ]
  br label %for.end

for.end:                                          ; preds = %for.cond.for.end_crit_edge, %entry
  %i.0.lcssa = phi i32 [ %split, %for.cond.for.end_crit_edge ], [ 0, %entry ]
  ret i32 %i.0.lcssa
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %feature = alloca [1 x [1000 x i32]], align 16
  %weight = alloca [1 x [1000 x i32]], align 16
  %hist = alloca [1 x [1000 x i32]], align 16
  %n = alloca [1 x i32], align 4
  call void @srand(i32 13) #3
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc20
  %i.02 = phi i32 [ 0, %entry ], [ %inc21, %for.inc20 ]
  %idxprom = sext i32 %i.02 to i64
  %arrayidx = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 %idxprom
  store i32 1000, i32* %arrayidx, align 4
  br label %for.body3

for.body3:                                        ; preds = %for.body, %for.inc
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 1000
  %idxprom4 = sext i32 %i.02 to i64
  %arrayidx5 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %feature, i64 0, i64 %idxprom4
  %idxprom6 = sext i32 %j.01 to i64
  %arrayidx7 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx5, i64 0, i64 %idxprom6
  store i32 %rem, i32* %arrayidx7, align 4
  %call8 = call i32 @rand() #3
  %rem9 = srem i32 %call8, 100
  %idxprom10 = sext i32 %i.02 to i64
  %arrayidx11 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %weight, i64 0, i64 %idxprom10
  %idxprom12 = sext i32 %j.01 to i64
  %arrayidx13 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  store i32 %rem9, i32* %arrayidx13, align 4
  %call14 = call i32 @rand() #3
  %rem15 = srem i32 %call14, 100
  %idxprom16 = sext i32 %i.02 to i64
  %arrayidx17 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %hist, i64 0, i64 %idxprom16
  %idxprom18 = sext i32 %j.01 to i64
  %arrayidx19 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx17, i64 0, i64 %idxprom18
  store i32 %rem15, i32* %arrayidx19, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %j.01, 1
  %cmp2 = icmp slt i32 %inc, 1000
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.inc
  br label %for.inc20

for.inc20:                                        ; preds = %for.end
  %inc21 = add nsw i32 %i.02, 1
  %cmp = icmp slt i32 %inc21, 1
  br i1 %cmp, label %for.body, label %for.end22

for.end22:                                        ; preds = %for.inc20
  %arrayidx25 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %feature, i64 0, i64 0
  %arraydecay = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx25, i32 0, i32 0
  %arrayidx27 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %weight, i64 0, i64 0
  %arraydecay28 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx27, i32 0, i32 0
  %arrayidx30 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %hist, i64 0, i64 0
  %arraydecay31 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx30, i32 0, i32 0
  %arrayidx33 = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 0
  %0 = load i32, i32* %arrayidx33, align 4
  %call34 = call i32 @_Z9histogramPiS_S_i(i32* %arraydecay, i32* %arraydecay28, i32* %arraydecay31, i32 %0)
  ret i32 0
}

; Function Attrs: nounwind
declare void @srand(i32) #2

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
