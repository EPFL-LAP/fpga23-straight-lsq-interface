; ModuleID = '.getTanh.cpp_mem2reg.ll'
source_filename = "src/getTanh.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z7getTanhPiS_(i32* %A, i32* %addr) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc
  %i.01 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %idxprom = sext i32 %i.01 to i64
  %arrayidx = getelementptr inbounds i32, i32* %addr, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %0 to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %A, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4
  %cmp3 = icmp sge i32 %1, 1
  br i1 %cmp3, label %if.then, label %if.else

if.then:                                          ; preds = %for.body
  br label %if.end

if.else:                                          ; preds = %for.body
  %mul = mul nsw i32 %1, %1
  %add = add nsw i32 %mul, 19
  %mul4 = mul nsw i32 %add, %1
  %mul5 = mul nsw i32 %mul4, %1
  %mul6 = mul nsw i32 3, %1
  %add7 = add nsw i32 %mul5, %mul6
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %result.1 = phi i32 [ 1, %if.then ], [ %add7, %if.else ]
  %idxprom8 = sext i32 %0 to i64
  %arrayidx9 = getelementptr inbounds i32, i32* %A, i64 %idxprom8
  store i32 %result.1, i32* %arrayidx9, align 4
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %inc = add nsw i32 %i.01, 1
  %cmp = icmp slt i32 %inc, 1000
  br i1 %cmp, label %for.body, label %for.end

for.end:                                          ; preds = %for.inc
  %result.0.lcssa = phi i32 [ %result.1, %for.inc ]
  ret i32 %result.0.lcssa
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %a = alloca [1 x [1000 x i32]], align 16
  %b = alloca [1 x [1000 x i32]], align 16
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc15
  %i.02 = phi i32 [ 0, %entry ], [ %inc16, %for.inc15 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body, %for.inc
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %idxprom = sext i32 %i.02 to i64
  %arrayidx = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 %idxprom
  %idxprom4 = sext i32 %j.01 to i64
  %arrayidx5 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %j.01, i32* %arrayidx5, align 4
  %idxprom6 = sext i32 %i.02 to i64
  %arrayidx7 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 %idxprom6
  %idxprom8 = sext i32 %j.01 to i64
  %arrayidx9 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 %j.01, i32* %arrayidx9, align 4
  %rem = srem i32 %j.01, 100
  %cmp10 = icmp eq i32 %rem, 0
  br i1 %cmp10, label %if.then, label %if.end

if.then:                                          ; preds = %for.body3
  %idxprom11 = sext i32 %i.02 to i64
  %arrayidx12 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 %idxprom11
  %idxprom13 = sext i32 %j.01 to i64
  %arrayidx14 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx12, i64 0, i64 %idxprom13
  store i32 0, i32* %arrayidx14, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body3
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %inc = add nsw i32 %j.01, 1
  %cmp2 = icmp slt i32 %inc, 1000
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.inc
  br label %for.inc15

for.inc15:                                        ; preds = %for.end
  %inc16 = add nsw i32 %i.02, 1
  %cmp = icmp slt i32 %inc16, 1
  br i1 %cmp, label %for.body, label %for.end17

for.end17:                                        ; preds = %for.inc15
  %arrayidx20 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0
  %arraydecay = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx20, i32 0, i32 0
  %arrayidx22 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 0
  %arraydecay23 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx22, i32 0, i32 0
  %call = call i32 @_Z7getTanhPiS_(i32* %arraydecay, i32* %arraydecay23)
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
