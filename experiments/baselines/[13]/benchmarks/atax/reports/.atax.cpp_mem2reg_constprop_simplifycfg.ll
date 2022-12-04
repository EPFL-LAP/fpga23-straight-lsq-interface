; ModuleID = '.atax.cpp_mem2reg_constprop.ll'
source_filename = "src/atax.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z4ataxPA20_iPiS1_S1_([20 x i32]* %A, i32* %x, i32* %y, i32* %tmp) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.end25, %entry
  %i.04 = phi i32 [ 0, %entry ], [ %inc29, %for.end25 ]
  %idxprom = sext i32 %i.04 to i64
  %arrayidx = getelementptr inbounds i32, i32* %tmp, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %t.02 = phi i32 [ %0, %for.body ], [ %add, %for.body3 ]
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %idxprom4 = sext i32 %i.04 to i64
  %arrayidx5 = getelementptr inbounds [20 x i32], [20 x i32]* %A, i64 %idxprom4
  %idxprom6 = sext i32 %j.01 to i64
  %arrayidx7 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx5, i64 0, i64 %idxprom6
  %1 = load i32, i32* %arrayidx7, align 4
  %idxprom8 = sext i32 %j.01 to i64
  %arrayidx9 = getelementptr inbounds i32, i32* %x, i64 %idxprom8
  %2 = load i32, i32* %arrayidx9, align 4
  %mul = mul nsw i32 %1, %2
  %add = add nsw i32 %t.02, %mul
  %inc = add nsw i32 %j.01, 1
  %cmp2 = icmp slt i32 %inc, 20
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.body3
  %t.0.lcssa = phi i32 [ %add, %for.body3 ]
  br label %for.body12

for.body12:                                       ; preds = %for.body12, %for.end
  %j.13 = phi i32 [ 0, %for.end ], [ %inc24, %for.body12 ]
  %idxprom13 = sext i32 %j.13 to i64
  %arrayidx14 = getelementptr inbounds i32, i32* %y, i64 %idxprom13
  %3 = load i32, i32* %arrayidx14, align 4
  %idxprom15 = sext i32 %i.04 to i64
  %arrayidx16 = getelementptr inbounds [20 x i32], [20 x i32]* %A, i64 %idxprom15
  %idxprom17 = sext i32 %j.13 to i64
  %arrayidx18 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx16, i64 0, i64 %idxprom17
  %4 = load i32, i32* %arrayidx18, align 4
  %mul19 = mul nsw i32 %4, %t.0.lcssa
  %add20 = add nsw i32 %3, %mul19
  %idxprom21 = sext i32 %j.13 to i64
  %arrayidx22 = getelementptr inbounds i32, i32* %y, i64 %idxprom21
  store i32 %add20, i32* %arrayidx22, align 4
  %inc24 = add nsw i32 %j.13, 1
  %cmp11 = icmp slt i32 %inc24, 20
  br i1 %cmp11, label %for.body12, label %for.end25

for.end25:                                        ; preds = %for.body12
  %idxprom26 = sext i32 %i.04 to i64
  %arrayidx27 = getelementptr inbounds i32, i32* %tmp, i64 %idxprom26
  store i32 %t.0.lcssa, i32* %arrayidx27, align 4
  %inc29 = add nsw i32 %i.04, 1
  %cmp = icmp slt i32 %inc29, 20
  br i1 %cmp, label %for.body, label %for.end30

for.end30:                                        ; preds = %for.end25
  %i.0.lcssa = phi i32 [ %inc29, %for.end25 ]
  ret i32 %i.0.lcssa
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %A = alloca [1 x [20 x [20 x i32]]], align 16
  %x = alloca [1 x [20 x i32]], align 16
  %y = alloca [1 x [20 x i32]], align 16
  %tmp = alloca [1 x [20 x i32]], align 16
  call void @srand(i32 13) #3
  br label %for.body

for.body:                                         ; preds = %for.inc29, %entry
  %i.03 = phi i32 [ 0, %entry ], [ %inc30, %for.inc29 ]
  br label %for.body3

for.body3:                                        ; preds = %for.inc26, %for.body
  %j.02 = phi i32 [ 0, %for.body ], [ %inc27, %for.inc26 ]
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 100
  %idxprom = sext i32 %i.03 to i64
  %arrayidx = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %x, i64 0, i64 %idxprom
  %idxprom4 = sext i32 %j.02 to i64
  %arrayidx5 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %rem, i32* %arrayidx5, align 4
  %idxprom6 = sext i32 %i.03 to i64
  %arrayidx7 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %y, i64 0, i64 %idxprom6
  %idxprom8 = sext i32 %j.02 to i64
  %arrayidx9 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 0, i32* %arrayidx9, align 4
  %idxprom10 = sext i32 %i.03 to i64
  %arrayidx11 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %tmp, i64 0, i64 %idxprom10
  %idxprom12 = sext i32 %j.02 to i64
  %arrayidx13 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  store i32 0, i32* %arrayidx13, align 4
  br label %for.body17

for.body17:                                       ; preds = %for.body17, %for.body3
  %x14.01 = phi i32 [ 0, %for.body3 ], [ %inc, %for.body17 ]
  %call18 = call i32 @rand() #3
  %rem19 = srem i32 %call18, 100
  %idxprom20 = sext i32 %i.03 to i64
  %arrayidx21 = getelementptr inbounds [1 x [20 x [20 x i32]]], [1 x [20 x [20 x i32]]]* %A, i64 0, i64 %idxprom20
  %idxprom22 = sext i32 %j.02 to i64
  %arrayidx23 = getelementptr inbounds [20 x [20 x i32]], [20 x [20 x i32]]* %arrayidx21, i64 0, i64 %idxprom22
  %idxprom24 = sext i32 %x14.01 to i64
  %arrayidx25 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx23, i64 0, i64 %idxprom24
  store i32 %rem19, i32* %arrayidx25, align 4
  %inc = add nsw i32 %x14.01, 1
  %cmp16 = icmp slt i32 %inc, 20
  br i1 %cmp16, label %for.body17, label %for.inc26

for.inc26:                                        ; preds = %for.body17
  %inc27 = add nsw i32 %j.02, 1
  %cmp2 = icmp slt i32 %inc27, 20
  br i1 %cmp2, label %for.body3, label %for.inc29

for.inc29:                                        ; preds = %for.inc26
  %inc30 = add nsw i32 %i.03, 1
  %cmp = icmp slt i32 %inc30, 1
  br i1 %cmp, label %for.body, label %for.end31

for.end31:                                        ; preds = %for.inc29
  %arrayidx33 = getelementptr inbounds [1 x [20 x [20 x i32]]], [1 x [20 x [20 x i32]]]* %A, i64 0, i64 0
  %arraydecay = getelementptr inbounds [20 x [20 x i32]], [20 x [20 x i32]]* %arrayidx33, i32 0, i32 0
  %arrayidx34 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %x, i64 0, i64 0
  %arraydecay35 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx34, i32 0, i32 0
  %arrayidx36 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %y, i64 0, i64 0
  %arraydecay37 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx36, i32 0, i32 0
  %arrayidx38 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %tmp, i64 0, i64 0
  %arraydecay39 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx38, i32 0, i32 0
  %call40 = call i32 @_Z4ataxPA20_iPiS1_S1_([20 x i32]* %arraydecay, i32* %arraydecay35, i32* %arraydecay37, i32* %arraydecay39)
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
